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

	bool Channel::hasUser(const std::string &user_name) const {
		auto lock = lockUsers();
		for (auto &user: users)
			if (user->name == user_name)
				return true;
		return false;
	}

	std::shared_ptr<User> Channel::findUser(const std::string &user_name) const {
		auto lock = lockUsers();
		for (auto &user: users)
			if (user->name == user_name)
				return user;
		return nullptr;
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
		auto lock = lockUsers();
		auto iter = hats.find(user);
		if (iter == hats.end()) {
			hats.insert({user, HatSet()});
			return hats.at(user);
		}

		return iter->second;
	}

	HatSet & Channel::getHats(const std::string &user_name) {
		if (auto user = findUser(user_name))
			return getHats(user);
		throw std::runtime_error("Couldn't find user " + user_name + " in channel " + server->id + "/" + name);
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

	std::string Channel::withHat(std::shared_ptr<User> user, bool include_space) {
		auto lock = lockUsers();
		Hat highest = getHats(user).highest();
		if (highest == Hat::None)
			return include_space? ' ' + user->name : user->name;
		return static_cast<char>(highest) + user->name;
	}

	Channel::operator std::string() const {
		return server->id + "/" + name;
	}

	std::shared_ptr<User> Channel::operator[](const std::string &user_name) {
		for (std::shared_ptr<User> user: users)
			if (user->name == user_name)
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
