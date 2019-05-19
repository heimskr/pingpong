#ifndef CORE_SERVER_H_
#define CORE_SERVER_H_

#include <string>
#include "irc.h"

namespace pingpong {
	class server {
		public:
			std::string hostname;
			int port;

			server(std::string hostname): hostname(hostname), port(irc::default_port) { }
			server(std::string hostname, int port): hostname(hostname), port(port) { }
			bool is_user();
	};
}

#endif
