#include <string>

#include "messages/privmsg.h"

namespace pingpong {
	privmsg_message::operator std::string() const {
		return "[" + chan->name + "] <" + who->name + "> " + content;
	}
}