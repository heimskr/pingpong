#ifndef PINGPONG_SOCK_NET_H_
#define PINGPONG_SOCK_NET_H_

#include <string>

#include <netdb.h>

namespace pingpong::net {
	class sock {
		friend class socket_buffer;

		private:
			struct addrinfo *info;
			int fd;
			bool connected = false;

		public:
			const std::string hostname;
			int port;

			sock(const std::string &hostname_, int port_);

			sock() = delete;
			sock(const sock &) = delete;
			sock & operator=(const sock &) = delete;

			~sock();

			/** Connects to the socket. */
			void connect();

			/** Sends a given number of bytes from a buffer through the socket. */
			ssize_t send(const void *, size_t);

			/** Reads a given number of bytes into a buffer from the socket. */
			ssize_t recv(void *, size_t);
	};
}

#endif
