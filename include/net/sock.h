#ifndef PINGPONG_SOCK_NET_H_
#define PINGPONG_SOCK_NET_H_

#include <string>

#include <netdb.h>

namespace pingpong::net {
	class sock {
		friend class socket_stream;
		friend class socket_buffer;

		private:
			struct addrinfo *info;
			int fd;

		public:
			const std::string hostname;
			int port;

			sock(const std::string &hostname_, int port_);

			sock() = delete;
			sock(const sock &) = delete;
			sock & operator=(const sock &) = delete;

			~sock();

			void connect();
	};
}

#endif
