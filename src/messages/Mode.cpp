#include "pingpong/core/Hats.h"

#include "pingpong/events/Mode.h"
#include "pingpong/events/HatsUpdated.h"

#include "pingpong/messages/Mode.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	ModeMessage::ModeMessage(const PingPong::Line &line_): Message(line_), Local("") {
		size_t middle;

		ModeSet::ModeType modeset_type = ModeSet::ModeType::Self;
		std::string modeset_main {}, modeset_extra {};

		// is_server() is true sometimes because the line source looks like a server name, even though it's a nick.
		// It doesn't actually indicate that the message source is the server.
		const bool is_server = line.source.isServer();

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
					throw BadMessage(line);
				middle_size = 1;
			} else if (line.source.nick != line.server->getNick()) {
				// The mode command, if the source is a nick, tells you what your current nick is. If it's different
				// from what we were assuming, we need to take the source as your new nick.
				// Rizon doesn't do this. Instead, it uses the server name as the source. We skip this check by testing
				// for a colon above; Rizon doesn't include a colon before the ModeSet.
				line.server->setNick(line.source.nick, true);
				line.server->removeUser("?");
			}
			
			modeset_main = line.parameters.substr(middle + middle_size);
			where = line.parameters.substr(0, middle);
		} else {
			// This is presumably a channel mode change. The parameters should look like "#chan -S".

			middle = line.parameters.find(' ');
			if (middle == std::string::npos)
				throw BadMessage(line);

			// Bans are in the form "+b nick!user@host", with an extra mask after the mode.
			std::string modestr {line.parameters.substr(middle + 1)};
			std::string extra;
			size_t space = modestr.find(' ');
			if (space != std::string::npos) {
				extra.assign(modestr, space + 1, std::string::npos);
				modestr.erase(space);
			}

			where = line.parameters.substr(0, middle);
			chan = line.server->getChannel(where, true);

			modeset_type = ModeSet::ModeType::Channel;
			modeset_main = modestr;
			modeset_extra = extra;
		}

		who = line.server->getUser(line.source, true, true);

		try {
			modeset = {modeset_type, modeset_main, modeset_extra};
		} catch (const std::invalid_argument &err) {
			DBG("Couldn't parse \"" << modeset.modes << "\" as a modestring: " << err.what());
			throw BadMessage(line);
		}
	}

	bool ModeMessage::operator()(Server *server) {
		if (modeset.type == ModeSet::ModeType::Channel) {
			if (Util::isValidNick(modeset.extra)) {
				// If the extra data in the ModeSet is a nickname, then presumably we need to add or remove a hat.
				// I'm not sure what other conditions could cause this situation.
				std::shared_ptr<User> extra_who = server->getUser(modeset.extra, true, true);
				const auto hat_end = HatSet::map.end();

				bool add = true;
				for (const std::unordered_set<char> &modeset_set: {modeset.added, modeset.removed}) {
					for (char c: modeset_set) {
						auto iter = HatSet::map.find(c);
						if (iter == hat_end)
							continue;

						HatSet &set = chan->getHats(extra_who), old_set = set;
						if (add) {
							set += iter->second;
						} else {
							set -= iter->second;
						}

						if (set != old_set)
							Events::dispatch<HatsUpdatedEvent>(extra_who, chan, old_set, set);
					}

					add = false;
				}
			} else if (modeset.extra.empty()) {
				(chan? chan : server->getChannel(where, true))->applyModes(modeset.removed, modeset.added);
			}
		} else {
			line.server->getSelf()->applyModes(modeset.removed, modeset.added);
		}

		Events::dispatch<ModeEvent>(server, where, who, modeset, line);
		return true;
	}

	ModeMessage::operator std::string() const {
		return "";
	}
}
