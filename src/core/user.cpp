#include "pingpong/core/user.h"

namespace pingpong {
	void user::rename(const std::string &new_name) {
		name = new_name;
	}

	std::ostream & operator<<(std::ostream &os, const pingpong::user &who) {
		os << std::string(who);
		return os;
	}

	bool user::is_self() const {
		return serv && name == serv->get_nick();
	}

	bool user::operator==(const user &other) const {
		return serv == other.serv && name == other.name;
	}
}
