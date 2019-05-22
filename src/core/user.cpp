#include "core/user.h"

namespace pingpong {
	void user::rename(const std::string &new_name) {
		name = new_name;
	}

	std::ostream & operator<<(std::ostream &os, const pingpong::user &who) {
		os << std::string(who);
		return os;
	}
}
