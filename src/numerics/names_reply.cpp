#include <string>
#include <utility>
#include <vector>

#include "pingpong/core/hats.h"
#include "pingpong/events/names_updated.h"
#include "pingpong/messages/numeric.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	bool numeric_message::handle_names_reply(server *serv) {
		names parsed;
		try {
			parsed = numeric_message::parse_names_reply(line.parameters);
		} catch (const std::invalid_argument &err) {
			WARN("Couldn't parse NAMES reply " << "["_d << line.parameters << "]"_d << ": " << err.what());
			return true;
		}

		std::string chanstr;
		channel::visibility vis;
		std::vector<std::pair<hat_set, std::string>> userlist;
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
			bool anyone_new = false;
			for (auto & [uhats, name]: userlist) {
				std::shared_ptr<user> uptr = serv->get_user(name, true);
				*uptr += chan;
				if (!chan->has_user(uptr)) {
					anyone_new = true;
					chan->add_user(uptr);
				}

				chan->set_hats(uptr, uhats);
			}

			if (anyone_new)
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

		std::string chanstr;
		std::vector<std::string> usersplit;
		try {
			chanstr = params.substr(sep + 3, colon - sep - 4);
			usersplit = formicine::util::split(params.substr(colon + 1), " ", true);
		} catch (const std::out_of_range &err) {
			throw std::invalid_argument("Invalid 353 message");
		}

		std::vector<std::pair<hat_set, std::string>> userlist;
		for (const std::string &userstr: usersplit) {
			hat_set userhats {};

			for (char ch: userstr) {
				if (hat_set::is_hat(ch)) {
					userhats += ch;
				} else {
					break;
				}
			}

			if (userhats == hat::none) {
				userlist.push_back({userhats, userstr});
			} else {
				userlist.push_back({userhats, userstr.substr(userhats.size())});
			}
		}

		return {chanstr, vis, userlist};
	}
}
