#include <stdexcept>

#include "core/local.h"

namespace pingpong {
	inline void local::check_destination() const {
		if (where.empty())
			throw std::runtime_error("Destination cannot be empty");
	}

	bool local::is_user() const {
		return where.front() != '#';
	}

	bool local::is_channel() const {
		if (where.empty())
			throw std::runtime_error("Destination cannot be empty");
		return where.front() == '#';
	}

	std::shared_ptr<user> local::get_user(server *serv) const {
		if (is_channel())
			return nullptr;
		// Because I don't want to assume that this would never be anything other than your nickname (as reasonable as
		// that assumption would be), I don't just return serv->get_self() here.
		return serv->get_user(where, true);
	}

	std::shared_ptr<channel> local::get_channel(server *serv) const {
		if (is_user())
			return nullptr;
		return serv->get_channel(where, true);
	}
}