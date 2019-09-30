#include <string>
#include <utility>
#include <vector>

#include "pingpong/events/event.h"
#include "pingpong/events/names_updated.h"

#include "pingpong/messages/numeric.h"

namespace pingpong {
	bool numeric_message::handle_names_reply(server *serv) {
		names parsed;
		try {
			parsed = numeric_message::parse_names_reply(line.parameters);
		} catch (const std::invalid_argument &err) {
			DBG("Couldn't parse NAMES reply");
			return true;
		}

		std::string chanstr;
		channel::visibility vis;
		std::vector<std::pair<hat, std::string>> userlist;
		std::tie(chanstr, vis, userlist) = parsed;

		std::shared_ptr<channel> chan = serv->get_channel(chanstr);

		bool should_clear = false;
		if (chan && serv->last_message && serv->last_message->get_name() != "_NUMERIC") {
			auto last_message = std::dynamic_pointer_cast<numeric_message>(serv->last_message);
			should_clear = last_message && last_message->type != numeric_type::names_reply;
		}

		if (should_clear) {
			// If the previous message was something other than a NAMES reply, reset the current user list.
			chan->users.clear();
		} else if (!chan) {
			WARN("Channel not in list: " << chanstr);
		}

		if (chan) {
			bool any = false;
			for (auto [uhat, name]: userlist) {
				std::shared_ptr<user> uptr = serv->get_user(name, true);
				*uptr += chan;
				if (!chan->has_user(uptr)) {
					any = true;
					chan->add_user(uptr);
					chan->hats.insert({uptr, uhat});
				}
			}

			if (any)
				events::dispatch<names_updated_event>(chan);
		}

		return true;
	}

	numeric_message::names numeric_message::parse_names_reply(const std::string &params) {
		size_t sep = params.find(" = ");
		channel::visibility vis = channel::visibility::pub;

		if (sep == std::string::npos) {
			sep = params.find(" * ");
			if (sep == std::string::npos) {
				sep = params.find(" @ ");
				vis = channel::visibility::secret;
			} else {
				vis = channel::visibility::priv;
			}
		}

		size_t colon = params.find(':');
		if (sep == std::string::npos || colon == std::string::npos || colon < sep)
			throw std::invalid_argument("Invalid 353 message");

		std::string chanstr, userstr;
		try {
			chanstr = params.substr(sep + 3, colon - sep - 4);
			userstr = params.substr(colon + 1);
			while (userstr.back() == ' ')
				userstr.pop_back();
		} catch (const std::out_of_range &err) {
			throw std::invalid_argument("Invalid 353 message");
		}

		std::vector<std::pair<hat, std::string>> userlist;
		while (userstr.length() > 0) {
			size_t space = userstr.find(' ');
			std::string to_add;
			if (space == std::string::npos) {
				to_add = userstr;
				userstr.clear();
			} else {
				to_add = userstr.substr(0, space);
				for (; userstr[space] == ' '; ++space);
				userstr.erase(0, space);
			}

			hat userhat = user::get_hat(to_add);
			if (userhat != hat::none)
				to_add.erase(0, 1);
			userlist.push_back({userhat, to_add});
		}

		return {chanstr, vis, userlist};
	}
}
