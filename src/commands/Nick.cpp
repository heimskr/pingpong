#include <string>

#include "pingpong/commands/Nick.h"

namespace PingPong {
	NickCommand::operator std::string() const {
		return "NICK " + nickname;
	}
}
