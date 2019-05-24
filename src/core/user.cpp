#include "core/user.h"

namespace pingpong {
	void user::rename(const std::string &new_name) {
		name = new_name;
	}

	std::ostream & operator<<(std::ostream &os, const pingpong::user &who) {
		os << std::string(who);
		return os;
	}

	user & user::operator+=(channel_ptr chan) {
		channels.insert(chan);
		return *this;
	}

	hat user::get_hat(char ch) {
		if (ch == hat::voiced) return hat::voiced;
		if (ch == hat::halfop) return hat::halfop;
		if (ch == hat::op)     return hat::op;
		if (ch == hat::admin)  return hat::admin;
		if (ch == hat::owner)  return hat::owner;
		return hat::none;
	}

	hat user::get_hat(const std::string &str) {
		return str.empty() || str[0] == ' '? hat::none : get_hat(str[0]);
	}
}
