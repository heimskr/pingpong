#include <string>

#include "ping.h"

namespace pingpong {
	std::string ping_response::get_command() const {
		return "PING";
	}

	ping_response::operator std::string() const {
		return "PING " + text;
	}
}