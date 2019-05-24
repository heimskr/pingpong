#include <string>

#include "messages/ping.h"

namespace pingpong {
	ping_message::operator std::string() const {
		return "PING " + content;
	}
}