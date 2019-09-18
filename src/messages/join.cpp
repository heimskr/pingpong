#include <iostream>
#include <string>

#include "core/server.h"
#include "events/join.h"
#include "events/names_updated.h"
#include "messages/join.h"

namespace pingpong {
	join_message::operator std::string() const {
		return who + " joined " + chan;
	}

	bool join_message::operator()(server *serv) {
		if (chan.empty() || chan.front() != '#')
			throw std::runtime_error("Invalid channel for join_message");
		
		std::shared_ptr<user> userptr = serv->get_user(who, true);
		std::shared_ptr<channel> chanptr = serv->get_channel(chan, true);

		if (!chanptr->has_user(userptr)) {
			chanptr->add_user(userptr);
			events::dispatch<names_updated_event>(chanptr);
		}

		events::dispatch<join_event>(userptr, chanptr);
		return true;
	}
}