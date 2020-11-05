#ifndef PINGPONG_SOCK_NET_H_
#define PINGPONG_SOCK_NET_H_

#include <string>

#include <netdb.h>

namespace PingPong::Net {
	class Sock {
		friend class SocketBuffer;

		private:
			static int sockCount;
			struct addrinfo *info;
			int netFD = -1, controlRead = -1, controlWrite = -1;
			bool connected = false;
			fd_set fds = {0};

			enum class ControlMessage: char {Close='C'};

		public:
			const std::string hostname;
			int port;

			Sock(const std::string &hostname_, int port_);

			Sock() = delete;
			Sock(const Sock &) = delete;
			Sock & operator=(const Sock &) = delete;

			~Sock();

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
