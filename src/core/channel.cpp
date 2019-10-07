#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <string>

#include "pingpong/core/defs.h"
#include "pingpong/core/channel.h"
#include "pingpong/core/server.h"

namespace pingpong {
	channel::channel(std::string name_, server *serv_): name(name_), serv(serv_) {
		if (name_.empty() || (name_[0] != '#' && name_[0] != '&'))
			throw std::invalid_argument("Invalid channel name");
	}

	channel::channel(std::string name_): channel(name_, nullptr) {}

	bool channel::has_server() const {
		return serv != nullptr;
	}

	bool channel::add_user(std::shared_ptr<user> user) {
		if (!has_user(user)) {
			users.push_back(user);
			return true;
		}

		return false;
	}

	bool channel::remove_user(std::shared_ptr<user> user) {
		auto iter = std::find(users.begin(), users.end(), user);
		if (iter != users.end()) {
			users.erase(iter);
			return true;
		}

		return false;
	}

	bool channel::rename_user(const std::string &old_nick, const std::string &new_nick) {
		// It's possible that the user has already been renamed in server::rename_user.
		if (!has_user(new_nick)) {
			for (std::shared_ptr<user> user: users) {
				if (user->name == old_nick) {
					user->rename(new_nick);
					return true;
				}
			}
		}

		return false;
	}

	bool channel::has_user(std::shared_ptr<user> user) const {
		return user && user->serv == serv && std::find(users.begin(), users.end(), user) != users.end();
	}

	bool channel::has_user(const std::string &name) const {
		for (std::shared_ptr<user> user: users) {
			if (user->name == name)
				return true;
		}
		
		return false;
	}

	bool channel::set_hats(std::shared_ptr<user> user, const hat_set &set) {
		auto iter = hats.find(user);

		if (!set) {
			if (iter != hats.end()) {
				hats.erase(iter);
				return true;
			}

			return false;
		}

		if (iter == hats.end()) {
			hats.insert({user, set});
			return false;
		}

		hats.erase(iter);
		hats.insert({user, set});
		return true;
	}

	hat_set & channel::get_hats(std::shared_ptr<user> user) {
		auto iter = hats.find(user);
		if (iter == hats.end()) {
			hats.insert({user, hat_set()});
			return hats.at(user);
		}

		return iter->second;
	}

	channel::operator std::string() const {
		return serv->id + "/" + name;
	}

	std::shared_ptr<user> channel::operator[](const std::string &name) {
		for (std::shared_ptr<user> user: users) {
			if (user->name == name)
				return user;
		}

		return nullptr;
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

