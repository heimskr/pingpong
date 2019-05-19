#include <string>

#include "user.h"

namespace pingpong {
	std::string user_command::to_string() const {
		return "USER " + username + " 0 * :" + realname;
	}
}
