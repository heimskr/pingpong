#include <string>

#include "user.h"

namespace pingpong {
	user_command::operator std::string() const {
		return "USER " + username + " 0 * :" + realname;
	}
}
