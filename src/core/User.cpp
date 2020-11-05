#include "pingpong/core/User.h"

namespace PingPong {
	void User::rename(const std::string &new_name) {
		name = new_name;
	}

	bool User::isSelf() const {
		return server && name == server->getNick();
	}

	bool User::operator==(const User &other) const {
		return server == other.server && name == other.name;
	}

	std::ostream & operator<<(std::ostream &os, const PingPong::User &user) {
		return os << std::string(user);
	}
}
