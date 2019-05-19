#ifndef CORE_SERVER_H_
#define CORE_SERVER_H_

#include <string>
#include "irc.h"

namespace pingpong {
	using std::string;
	using serv_ptr = std::shared_ptr<server>;

	class server {
		public:
			enum stage {
				// Connecting to an IRC server occurs in multiple stages.
				unconnected, // At first, a socket hasn't even been connected yet.
				user,        // Once the socket is connected, you need to send a USER message.
				nick,        // After the USER message is sent, you need to declare your nickname.
				registered   // After the nickname has been successfully declared, the connection is ready.
			};

			string hostname;
			int port;

			server(string hostname): hostname(hostname), port(irc::default_port) {}
			server(string hostname, int port): hostname(hostname), port(port) {}
			bool is_user();

			void quote(string raw);
	};
}

#endif
