#include "nick.h"

namespace pingpong {
	using std::string;

	string nick_command::to_string() const {
		return "NICK :" + nickname;
	}
}
