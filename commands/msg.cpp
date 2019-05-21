#include <string>

#include "commands/msg.h"

namespace pingpong {
	msg_command::operator std::string() const {
		return "PRIVMSG " + destination.name + " :" + text;
	}
}
