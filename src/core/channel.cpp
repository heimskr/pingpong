#include <algorithm>
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
		// It's possible that the user has already been renamed in server::rename_user.
		if (!has_user(new_nick)) {
			for (user_ptr user: users) {
				if (user->name == old_nick) {
					user->rename(new_nick);
					return true;
				}
			}
		}

		return false;
	}

	bool channel::has_user(user_ptr user) const {
		return user && user->serv == serv && std::find(users.begin(), users.end(), user) != users.end();
	}

	bool channel::has_user(const std::string &name) const {
		for (user_ptr user: users) {
			if (user->name == name)
				return true;
		}
		
		return false;
	}

	hat channel::get_hat(user_ptr user) const {
		auto iter = hats.find(user);
		return iter == hats.end()? hat::none : iter->second;
	}

	channel::operator std::string() const {
		return serv->hostname + "/" + name;
	}

	user_ptr channel::operator[](const std::string &name) {
		for (user_ptr user: users) {
			if (user->name == name)
				return user;
		}

		return nullptr;
	}

	channel & channel::operator+=(user_ptr user) {
		if (!has_user(user))
			users.push_back(user);
		return *this;
	}

	channel & channel::operator-=(user_ptr user) {
		auto iter = std::find(users.begin(), users.end(), user);
		if (iter != users.end())
			users.erase(iter);
		return *this;
	}

	bool channel::operator==(const std::string &str) const {
		return name == str;
	}

	bool channel::operator!=(const std::string &str) const {
		return name != str;
	}

	bool channel::operator==(const channel &chan) const {
		return name == chan.name && serv == chan.serv;
	}

	bool channel::operator!=(const channel &chan) const {
		return name != chan.name || serv != chan.serv;
	}

	bool channel::operator<(const channel &chan) const {
		return name < chan.name;
	}

	std::ostream & operator<<(std::ostream &os, const channel &chan) {
		os << std::string(chan);
		return os;
	}
}

