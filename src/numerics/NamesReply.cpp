#include <string>
#include <utility>
#include <vector>

#include "pingpong/core/Hats.h"
#include "pingpong/events/NamesUpdated.h"
#include "pingpong/messages/Numeric.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	bool NumericMessage::handleNamesReply(Server *server) {
		Names parsed;
		try {
			parsed = NumericMessage::parseNamesReply(line.parameters);
		} catch (const std::invalid_argument &err) {
			WARN("Couldn't parse NAMES reply " << "["_d << line.parameters << "]"_d << ": " << err.what());
			return true;
		}

		std::string chanstr;
		Channel::Visibility vis;
		std::vector<std::pair<HatSet, std::string>> userlist;
		std::tie(chanstr, vis, userlist) = parsed;

		std::shared_ptr<Channel> channel = server->getChannel(chanstr);

		bool should_clear = false;
		if (channel && server->lastMessage && server->lastMessage->getName() != "_NUMERIC") {
			auto lastMessage = std::dynamic_pointer_cast<NumericMessage>(server->lastMessage);
			should_clear = lastMessage && lastMessage->type != NumericType::NamesReply;
		}

		if (should_clear) {
			// If the previous message was something other than a NAMES reply, reset the current user list.
			channel->users.clear();
		} else if (!channel) {
			WARN("Channel not in list: " << chanstr);
		}

		if (channel) {
			bool anyone_new = false;
			for (auto &[uhats, name]: userlist) {
				std::shared_ptr<User> uptr = server->getUser(name, true);
				*uptr += channel;
				if (!channel->hasUser(uptr)) {
					anyone_new = true;
					channel->addUser(uptr);
				}

				channel->setHats(uptr, uhats);
			}

			if (anyone_new)
				Events::dispatch<NamesUpdatedEvent>(channel);
		}

		return true;
	}

	NumericMessage::Names NumericMessage::parseNamesReply(const std::string &params) {
		size_t sep = params.find(" = ");
		Channel::Visibility vis = Channel::Visibility::Public;

		if (sep == std::string::npos) {
			sep = params.find(" * ");
			if (sep == std::string::npos) {
				sep = params.find(" @ ");
				vis = Channel::Visibility::Secret;
			} else {
				vis = Channel::Visibility::Private;
			}
		}

		size_t colon = params.find(':');
		if (sep == std::string::npos || colon == std::string::npos || colon < sep)
			throw std::invalid_argument("Invalid 353 message");

		std::string chanstr;
		std::vector<std::string> usersplit;
		try {
			chanstr = params.substr(sep + 3, colon - sep - 4);
			usersplit = formicine::util::split(params.substr(colon + 1), " ", true);
		} catch (const std::out_of_range &err) {
			throw std::invalid_argument("Invalid 353 message");
		}

		std::vector<std::pair<HatSet, std::string>> userlist;
		for (const std::string &userstr: usersplit) {
			HatSet userhats {};

			for (char ch: userstr) {
				if (HatSet::isHat(ch))
					userhats += ch;
				else
					break;
			}

			if (userhats == Hat::None)
				userlist.push_back({userhats, userstr});
			else
				userlist.push_back({userhats, userstr.substr(userhats.size())});
		}

		return {chanstr, vis, userlist};
	}
}
