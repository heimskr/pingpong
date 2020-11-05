#include <algorithm>
#include <ostream>
#include <stdexcept>
#include <string>

#include "pingpong/core/Defs.h"
#include "pingpong/core/Channel.h"
#include "pingpong/core/Server.h"

namespace PingPong {
	Channel::Channel(const std::string &name_, Server *server_): name(name_), server(server_) {
		if (name_.empty() || (name_[0] != '#' && name_[0] != '&'))
			throw std::invalid_argument("Invalid channel name");
	}

	bool Channel::hasServer() const {
		return server != nullptr;
	}

	bool Channel::addUser(std::shared_ptr<User> user) {
		auto lock = lockUsers();

		if (!hasUser(user)) {
			users.push_front(user);
			return true;
		}

		return false;
	}

	bool Channel::removeUser(std::shared_ptr<User> user) {
		auto lock = lockUsers();

		auto iter = std::find(users.begin(), users.end(), user);
		if (iter != users.end()) {
			users.erase(iter);
			return true;
		}

		return false;
	}

	bool Channel::renameUser(const std::string &old_nick, const std::string &new_nick) {
		auto lock = lockUsers();
		// It's possible that the user has already been renamed in Server::renameUser.
		if (!hasUser(new_nick)) {
			for (const std::shared_ptr<User> &user: users) {
				if (user->name == old_nick) {
					user->rename(new_nick);
					return true;
				}
			}
		}

		return false;
	}

	bool Channel::hasUser(std::shared_ptr<User> user) const {
		return user && user->server == server && std::find(users.begin(), users.end(), user) != users.end();
	}

	bool Channel::hasUser(const std::string &name) const {
		for (std::shared_ptr<User> user: users) {
			if (user->name == name)
				return true;
		}
		
		return false;
	}

	bool Channel::setHats(std::shared_ptr<User> user, const HatSet &set) {
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

	HatSet & Channel::getHats(std::shared_ptr<User> user) {
		auto iter = hats.find(user);
		if (iter == hats.end()) {
			hats.insert({user, HatSet()});
			return hats.at(user);
		}

		return iter->second;
	}

	bool Channel::sendToFront(std::shared_ptr<User> user) {
		if (!user)
			return false;

		auto lock = lockUsers();

		// No need to do anything if the user is already at the front.
		if (users.front() == user)
			return false;

		// Search for the user in the list. If found, erase the user at the found position and push them to the front.
		for (auto iter = users.begin(), end = users.end(); iter != end; ++iter) {
			if (*iter == user) {
				users.erase(iter);
				users.push_front(user);
				return true;
			}
		}

		return false;
	}

	bool Channel::sendToFront(const std::string &nick) {
		return sendToFront(server->getUser(nick, false, false));
	}

	void Channel::sortUsers() {
		auto lock = lockUsers();
		users.sort([&](const std::shared_ptr<User> &left, const std::shared_ptr<User> &right) {
			const auto mismatch = std::mismatch(left->name.cbegin(), left->name.cend(), right->name.cbegin(),
				right->name.cend(), [](const unsigned char lchar, const unsigned char rchar) {
					return tolower(lchar) == tolower(rchar);
				});

			if (mismatch.second != right->name.cend())
				return false;

			return mismatch.first == left->name.cend() || tolower(*mismatch.first) < tolower(*mismatch.second);
		});
	}

	Channel::operator std::string() const {
		return server->id + "/" + name;
	}

	std::shared_ptr<User> Channel::operator[](const std::string &name) {
		for (std::shared_ptr<User> user: users)
			if (user->name == name)
				return user;
		return nullptr;
	}

	bool Channel::operator==(const std::string &str) const {
		return name == str;
	}

	bool Channel::operator!=(const std::string &str) const {
		return name != str;
	}

	bool Channel::operator==(const Channel &channel) const {
		return name == channel.name && server == channel.server;
	}

	bool Channel::operator!=(const Channel &channel) const {
		return name != channel.name || server != channel.server;
	}

	bool Channel::operator<(const Channel &channel) const {
		return name < channel.name;
	}

	std::ostream & operator<<(std::ostream &os, const Channel &channel) {
		return os << std::string(channel);
	}
}
