#ifndef CORE_MASK_H_
#define CORE_MASK_H_

#include <string>

namespace pingpong {
	class mask {
		public:
			std::string nick, user, host;

			mask(std::string nick_, std::string user_, std::string host_): nick(nick_), user(user_), host(host_) {}
			mask(std::string combined);

			bool is_full() const;
			bool is_empty() const;

			operator std::string() const;
	};
}

#endif
