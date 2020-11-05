#include <string>

#include "pingpong/commands/Pong.h"

namespace PingPong {
	PongCommand::operator std::string() const {
		return "PONG :" + token;
	}
}
