#ifndef CORE_IRC_H_
#define CORE_IRC_H_

#include <vector>
#include "core.h"

namespace pingpong {
	class irc {
		public:
			static constexpr int default_port = 6667;
			std::vector<pingpong::server> servers;
	};
}

#endif
