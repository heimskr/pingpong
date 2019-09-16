#ifndef PINGPONG_SOCK_NET_H_
#define PINGPONG_SOCK_NET_H_

#include <string>

#include <netdb.h>

namespace pingpong::net {
	class sock {
		friend class socket_buffer;

		private:
			struct addrinfo *info;
			int net_fd = -1, control_fd = -1;
			bool connected = false;
			fd_set fds;

			enum class control_message: char {close='C'};

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

			/** Closes the socket. */
			void close();

			/** Sends a given number of bytes from a buffer through the socket and returns the number of bytes sent. */
			ssize_t send(const void *, size_t);

			/** Reads a given number of bytes into a buffer from the socket and returns the number of bytes read. */
			ssize_t recv(void *, size_t);
	};
}

#endif
