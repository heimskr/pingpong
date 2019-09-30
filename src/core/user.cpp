#include "pingpong/core/user.h"

namespace pingpong {
	void user::rename(const std::string &new_name) {
		name = new_name;
	}

	std::ostream & operator<<(std::ostream &os, const pingpong::user &who) {
		os << std::string(who);
		return os;
	}

	hat user::get_hat(char ch) {
		if (ch == static_cast<char>(hat::voiced)) return hat::voiced;
		if (ch == static_cast<char>(hat::halfop)) return hat::halfop;
		if (ch == static_cast<char>(hat::op))     return hat::op;
		if (ch == static_cast<char>(hat::admin))  return hat::admin;
		if (ch == static_cast<char>(hat::owner))  return hat::owner;
		return hat::none;
	}

	hat user::get_hat(const std::string &str) {
		return str.empty() || str[0] == ' '? hat::none : get_hat(str[0]);
	}

	bool user::is_self() const {
		return serv && name == serv->get_nick();
	}

	bool user::operator==(const user &other) const {
		return serv == other.serv && name == other.name;
	}
}
