#include "user.h"

namespace pingpong {
	using std::string;

	string user_command::to_string() const {
		return "USER " + username + " 0 * :" + realname;
	}
}