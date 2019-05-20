#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <string>

#include "defs.h"
#include "server.h"

namespace pingpong {
	class channel {
		public:
			std::string name;
			server_ptr serv;

			channel(std::string, server_ptr);
			channel(std::string);
			bool is_user();

			operator std::string() const { return name; }
	};
}

#endif
