#include "commands/kick.h"

namespace pingpong {
	kick_command::operator std::string() const {
		return "KICK " + chan + " " + whom + " :" + reason;
	}
}
