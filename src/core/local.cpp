#include <stdexcept>

#include "pingpong/core/Local.h"

namespace PingPong {
	inline void Local::checkDestination() const {
		if (where.empty())
			throw std::runtime_error("Destination cannot be empty");
	}

	bool Local::isUser() const {
		return where.front() != '#' && where.front() != '&';
	}

	bool Local::isChannel() const {
		if (where.empty())
			throw std::runtime_error("Destination cannot be empty");
		return where.front() == '#' || where.front() == '&';
	}

	std::shared_ptr<User> Local::getUser(Server *server, bool update_case) const {
		if (isChannel())
			return nullptr;
		// Because I don't want to assume that this would never be anything other than your nickname (as reasonable as
		// that assumption would be), I don't just return server->getSelf() here.
		return server->getUser(where, true, update_case);
	}

	std::shared_ptr<Channel> Local::getChannel(Server *server) const {
		if (isUser())
			return nullptr;
		return server->getChannel(where, true);
	}
}
