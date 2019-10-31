#include "pingpong/core/hats.h"

#include "pingpong/events/mode.h"
#include "pingpong/events/hats_updated.h"

#include "pingpong/messages/mode.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	mode_message::mode_message(const pingpong::line &line_): message(line_), local("") {
		size_t middle;

		modeset::mode_type mset_type = modeset::mode_type::self;
		std::string mset_main {}, mset_extra {};

		// is_server() is true sometimes because the line source looks like a server name, even though it's a nick.
		// It doesn't actually indicate that the message source is the server.
		const bool is_server = line.source.is_server();

		// On some servers, the user/host are also filled in with yours. For these situtations, it's necessary to check
		// whether your nick is setting modes on itself.
		const bool is_self = line.parameters.substr(0, line.parameters.find(" :")) == line.source.nick;

		if (is_server || is_self) {
			// If the server notifies you of a user mode change, the mask includes your nick and sometimes your user and
			// host. The parameters will look like "pingpong :+iwx".

			middle = line.parameters.find(" :");
			size_t middle_size = 2;
			if (middle == std::string::npos) {
				// Rizon doesn't insert a colon for whatever reason.
				middle = std::min(line.parameters.find(" +"), line.parameters.find(" -"));
				if (middle == std::string::npos)
					throw bad_message(line);
				middle_size = 1;
			} else if (line.source.nick != line.serv->get_nick()) {
				// The mode command, if the source is a nick, tells you what your current nick is. If it's different
				// from what we were assuming, we need to take the source as your new nick.
				// Rizon doesn't do this. Instead, it uses the server name as the source. We skip this check by testing
				// for a colon above; Rizon doesn't include a colon before the modeset.
				line.serv->set_nick(line.source.nick, true);
				DBG("Removing fake user.");
				line.serv->remove_user("?");
			}
			
			mset_main = line.parameters.substr(middle + middle_size);
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

			mset_type = modeset::mode_type::channel;
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
				// I'm not sure what other conditions could cause this situation.
				std::shared_ptr<user> who = serv->get_user(mset.extra, true);
				const auto hat_end = hat_set::map.end();

				bool add = true;
				for (const std::unordered_set<char> &mset_set: {mset.added, mset.removed}) {
					for (char c: mset_set) {
						auto iter = hat_set::map.find(c);
						if (iter == hat_end)
							continue;

						hat_set &set = chan->get_hats(who), old_set = set;
						if (add) {
							set += iter->second;
						} else {
							set -= iter->second;
						}

						if (set != old_set)
							events::dispatch<hats_updated_event>(who, chan, old_set, set);
					}

					add = false;
				}
			} else if (mset.extra.empty()) {
				(chan? chan : serv->get_channel(where, true))->apply_modes(mset.removed, mset.added);
			}
		} else {
			line.serv->get_self()->apply_modes(mset.removed, mset.added);
		}

		events::dispatch<mode_event>(serv, where, who, mset, line);
		return true;
	}

	mode_message::operator std::string() const {
		return "";
	}
}
