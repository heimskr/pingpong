#include <string>

#include "messages/ping.h"

namespace pingpong {
	std::string ping_message::get_command() const {
		return "PING";
	}

	ping_message::operator std::string() const {
		return "PING " + text;
	}
}