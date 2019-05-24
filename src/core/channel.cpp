#include <ostream>
#include <stdexcept>
#include <string>

#include "core/defs.h"
#include "core/channel.h"
#include "core/server.h"

namespace pingpong {
	channel::channel(std::string name_, server_ptr serv_): name(name_), serv(serv_) {
		if (name_ == "") throw std::invalid_argument("Invalid channel name");
	}

	channel::channel(std::string name_): channel(name_, nullptr) {}

	bool channel::is_user() const {
		return name[0] != '#';
	}

	bool channel::has_server() const {
		return serv != nullptr;
	}

	bool channel::rename_user(const std::string &old_nick, const std::string &new_nick) {
		if (users.count(new_nick) > 0)
			throw user_exists_error(serv, old_nick, new_nick);

		auto iter = users.find(old_nick);
		if (iter == users.end())
			return false;

		iter->second->rename(new_nick);
		std::swap(users[old_nick], iter->second);
		users.erase(iter);
		return true;
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

