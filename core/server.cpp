#include <string>

#include "server.h"

namespace pingpong {
	std::string server::to_string() const {
		return port != irc::default_port? hostname + ":" + std::to_string(port) : hostname;
	}
}
