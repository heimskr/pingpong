#include <string>

#include "messages/privmsg.h"

namespace pingpong {
	std::string privmsg_message::get_command() const {
		return "PRIVMSG";
	}

	privmsg_message::operator std::string() const {
		return "[" + source + "] <" + line.source.nick + "> " + content;
	}
}