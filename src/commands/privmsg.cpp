#include "commands/privmsg.h"
#include "events/event.h"
#include "events/privmsg.h"

namespace pingpong {
	privmsg_command::operator std::string() const {
		return "PRIVMSG " + destination->name + " :" + message;
	}

	void privmsg_command::send() {
		command::send();
		events::dispatch<privmsg_event>(serv->get_self(), destination, message);
	}
}
