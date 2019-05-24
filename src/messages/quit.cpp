#include <string>

#include "messages/quit.h"

namespace pingpong {
	quit_message::operator std::string() const {
		return line.source.nick + " quit [" + content + "]";
	}
}
