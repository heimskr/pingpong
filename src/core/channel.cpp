#include <ostream>
#include <stdexcept>
#include <string>

#include "core/ppdefs.h"
#include "core/channel.h"
#include "core/server.h"

namespace pingpong {
	channel::channel(std::string name_, server_ptr serv_): name(name_), serv(serv_) {
		if (name_.empty() || name_[0] != '#')
			throw std::invalid_argument("Invalid channel name");
	}

	channel::channel(std::string name_): channel(name_, nullptr) {}

	bool channel::has_server() const {
		return serv != nullptr;
	}

	bool channel::rename_user(const std::string &old_nick, const std::string &new_nick) {
		if (users.count(new_nick) > 0)
			throw user_exists_error(serv, old_nick, new_nick);

		auto iter = users.find(old_nick);
		if (iter == users.end())
			return false;

		user_ptr uptr = iter->second;
		uptr->rename(new_nick);
		auto nh = users.extract(iter);
		nh.key() = new_nick;
		users.insert(std::move(nh));

		return true;
	}

	bool channel::has_user(user_ptr user) const {
		return user && user->serv == serv && users.count(user->name) == 1 && users.at(user->name) == user;
	}

	bool channel::has_user(const std::string &name) const {
		return users.count(name) == 1;
	}

	user_ptr channel::operator[](const std::string &str) {
		return users.count(str) > 0? users.at(str) : nullptr;
	}

	bool channel::operator==(const std::string &str) const {
		return name == str;
	}

	bool channel::operator==(const channel &chan) const {
		return name == chan.name && serv == chan.serv;
	}

	bool channel::operator<(const channel &chan) const {
		return name < chan.name;
	}

	std::ostream & operator<<(std::ostream &os, const channel &chan) {
		os << std::string(chan);
		return os;
	}
}

