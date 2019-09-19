#include "events/mode.h"
#include "messages/mode.h"

namespace pingpong {
	mode_message::mode_message(const pingpong::line &line_): message(line_), local("") {
		size_t middle;
		if (line.source.is_server() && line.source.nick == line.serv->get_nick()) {
			// If the server notifies you of a user mode change, the mask is just your nick without a user or host.
			// The parameters will look like "pingpong :+iwx".
			middle = line.parameters.find(" :");
			if (middle == std::string::npos)
				throw bad_message(line);
			
			mset = {modeset::mode_type::self, line.parameters.substr(middle + 2), ""};
			where = line.parameters.substr(0, middle);
		} else {
			// This is presumably a channel mode change. The parameters should look like "#chan -S".
			middle = line.parameters.find(' ');
			if (middle == std::string::npos)
				throw bad_message(line);

			// Bans are in the form "+b nick!user@host", with an extra mask after the mode.
			std::string modestr {line.parameters.substr(middle + 1)};
			std::string extra;
			size_t space = modestr.find(' ');
			if (space != std::string::npos) {
				extra.assign(modestr, space + 1, std::string::npos);
				modestr.erase(space);
			}

			where = line.parameters.substr(0, middle);
			chan = line.serv->get_channel(where, true);
			mset = modeset(modeset::mode_type::channel, modestr, extra);
		}

		who = line.serv->get_user(line.source, true);

		try {
			mset.process();
		} catch (const std::invalid_argument &err) {
			DBG("Couldn't parse \"" << mset.modes << "\" as a modestring: " << err.what());
			throw bad_message(line);
		}
	}

	bool mode_message::operator()(server *serv) {
		if (mset.type == modeset::mode_type::channel) {
			if (util::is_valid_nick(mset.extra)) {
				// If the extra data in the modeset is a nickname, then presumably we need to add or remove a hat.
				// I'm not sure what other situations could cause this situation.
				std::shared_ptr<user> who = serv->get_user(mset.extra, true);
				std::map<std::shared_ptr<user>, hat> &chan_hats = chan->hats;
				const auto hat_end = hat_map.end();

				bool add = true;

				for (const std::unordered_set<char> &mset_set: {mset.added, mset.removed}) {
					for (char c: mset_set) {
						auto iter = hat_map.find(c);
						if (iter == hat_end)
							continue;

						hat new_hat = iter->second;

						// Try to find the user's hat in this channel. If one can't be found, assume the user has none.
						hat old_hat = hat::none;
						auto old_iter = chan_hats.find(who);
						if (old_iter != chan_hats.end())
							old_hat = old_iter->second;

						// If adding, we set the user's hat to the new hat if it outranks the old one.
						// If removing, we unset the user's hat if the new hat is the same as the old one.
						// (If the new hat were a lower rank, it wouldn't affect the highest hat; if it were higher
						// rank, our info's out of date anyway.)
						if (add && old_hat < new_hat) {
							DBG("Upgrading " << who->name << " from "
								<< ansi::wrap(std::string(1, static_cast<char>(old_hat)), ansi::style::underline)
								<< " to "
								<< ansi::wrap(std::string(1, static_cast<char>(new_hat)), ansi::style::underline)
								<< " in " << chan->name);
							chan_hats.insert({who, new_hat});
						} else if (!add && old_hat == new_hat) {
							DBG("Removing hat "
								<< ansi::wrap(std::string(1, static_cast<char>(old_hat)), ansi::style::underline)
								<< " from " << who->name << " in " << chan->name);
							chan_hats.erase(who);
						}
					}

					add = false;
				}
			} else if (mset.extra.empty()) {
				(chan? chan : serv->get_channel(where, true))->apply_modes(mset.removed, mset.added);
			}
		} else {
			line.serv->get_self()->apply_modes(mset.removed, mset.added);
		}

		events::dispatch<mode_event>(serv, where, who, mset);
		return true;
	}

	mode_message::operator std::string() const {
		return "";
	}
}
