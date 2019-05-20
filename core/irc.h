#ifndef CORE_IRC_H_
#define CORE_IRC_H_

#include <vector>

#include "defs.h"

namespace pingpong {
	class irc {
		private:
			std::vector<server> servers;

		public:
			static constexpr int default_port = 6667;
	};
}

#endif
