#include "events/mode.h"
#include "events/hat_updated.h"

#include "messages/mode.h"

namespace pingpong {
	mode_message::mode_message(const pingpong::line &line_): message(line_), local("") {
		size_t middle;

		modeset::mode_type mset_type = modeset::mode_type::self;
		std::string mset_main {}, mset_extra {};

		if (line.source.is_server() && line.source.nick == line.serv->get_nick()) {
			// If the server notifies you of a user mode change, the mask is just your nick without a user or host.
			// The parameters will look like "pingpong :+iwx".
			middle = line.parameters.find(" :");
			if (middle == std::string::npos)
				throw bad_message(line);
			
			mset_main = line.parameters.substr(middle + 2);
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

			mset_type = modeset::mode_type::self;
			mset_main = modestr;
			mset_extra = extra;
		}

		who = line.serv->get_user(line.source, true);

		try {
			mset = {mset_type, mset_main, mset_extra};
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
				// TODO: try to remember all modes for every user in the channel, not just the last seen hat.
				// Right now, when a hat is removed, it's assumed the user ends up with no hat, but this isn't always
				// true; if someone is +ho and you deop them, their hat won't disappear but will change from @ to %.
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
							chan_hats.erase(who);
							chan_hats.insert({who, new_hat});
							events::dispatch<hat_updated_event>(who, chan, old_hat, new_hat);
						} else if (!add && old_hat == new_hat) {
							chan_hats.erase(who);
							events::dispatch<hat_updated_event>(who, chan, old_hat, new_hat);
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
