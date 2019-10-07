#ifndef CORE_MASK_H_
#define CORE_MASK_H_

#include <string>

namespace pingpong {
	class mask {
		public:
			std::string nick {}, user {}, host {};

			mask() = default;

			mask(const std::string &nick_, const std::string &user_, const std::string &host_):
				nick(nick_), user(user_), host(host_) {}

			mask(const std::string &combined);

			bool is_full() const;
			bool is_empty() const;

			/** If a message comes directly from the server instead of from a user, the user and host will be empty. */
			bool is_server() const;

			operator bool() const { return !is_empty(); }
			operator std::string() const;
	};
}

#endif
