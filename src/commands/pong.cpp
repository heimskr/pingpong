#include <string>

#include "pingpong/commands/pong.h"

namespace pingpong {
	pong_command::operator std::string() const {
		return "PONG :" + token;
	}
}
