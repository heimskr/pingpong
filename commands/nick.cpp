#include "nick.h"

namespace pingpong {
	std::string nick_command::to_string() const {
		return "NICK :" + nickname;
	}
}
