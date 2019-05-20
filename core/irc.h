#ifndef CORE_IRC_H_
#define CORE_IRC_H_

#include <vector>

#include "defs.h"

namespace pingpong {
	class irc {
		private:
			std::vector<server_ptr> servers;

		public:
			static constexpr int default_port = 6667;
			std::string username, realname;
			irc(): irc("pingpong", "PingPong IRC") {}
			irc(std::string user, std::string real): username(user), realname(real) {}
	};
}

#endif
