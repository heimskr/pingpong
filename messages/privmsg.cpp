#include <string>

#include "messages/privmsg.h"

namespace pingpong {
	privmsg_message::operator std::string() const {
		return "[" + source + "] <" + line.source.nick + "> " + content;
	}
}