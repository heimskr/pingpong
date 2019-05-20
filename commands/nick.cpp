#include <string>

#include "commands/nick.h"

namespace pingpong {
	nick_command::operator std::string() const {
		return "NICK :" + nickname;
	}
}
