#ifndef CORE_MASK_H_
#define CORE_MASK_H_

#include <string>

namespace PingPong {
	class Mask {
		public:
			std::string nick {}, user {}, host {};

			Mask() = default;

			Mask(const std::string &nick_, const std::string &user_, const std::string &host_):
				nick(nick_), user(user_), host(host_) {}

			Mask(const std::string &combined);

			bool isFull() const;
			bool isEmpty() const;

			/** If a message comes directly from the server instead of from a user, the user and host will be empty. */
			bool isServer() const;

			operator bool() const { return !isEmpty(); }
			operator std::string() const;
	};
}

#endif
