#include "pingpong/commands/privmsg.h"

#include "pingpong/events/error.h"
#include "pingpong/events/privmsg.h"

namespace pingpong {
	privmsg_command::privmsg_command(server *serv_, std::string where_, std::string message_):
	command(serv_), local(where_), message(message_) {
		if (where.empty())
			throw std::runtime_error("Destination is empty in privmsg_command");
	}

	privmsg_command::operator std::string() const {
		return "PRIVMSG " + where + " :" + message;
	}

	bool privmsg_command::send() {
		if (command::send()) {
			if (is_channel()) {
				std::shared_ptr<channel> chan = get_channel(serv);
				if (chan) {
					if (!hidden)
						events::dispatch<privmsg_event>(serv->get_self(), chan, message);
				} else {
					events::dispatch<error_event>("Can't send message: channel is null", false);
				}
			} else if (!hidden) {
				events::dispatch<privmsg_event>(serv->get_self(), get_user(serv), message);
			}

			return true;
		}

		return false;
	}
}
