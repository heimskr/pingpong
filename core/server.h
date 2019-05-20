#ifndef CORE_SERVER_H_
#define CORE_SERVER_H_

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"

#include "defs.h"
#include "channel.h"
#include "irc.h"
#include "responses/response.h"

using Poco::Net::SocketAddress, Poco::Net::StreamSocket, Poco::Net::SocketStream;

namespace pingpong {
	class server {
		private:
			StreamSocket socket;
			std::shared_ptr<SocketStream> stream;
			irc &parent;

			void cleanup(std::unique_lock<std::mutex> &);
			void cleanup();
			void work();
			void process_line(const pingpong::line &);

		public:
			enum stage {
				// Connecting to an IRC server occurs in multiple stages.
				unconnected, // At first, a socket hasn't even been connected yet.
				user,        // Once the socket is connected, you need to send a USER message.
				nick,        // After the USER message is sent, you need to declare your nickname.
				registered,  // After the nickname has been successfully declared, the connection is ready.
				dead         // After the server has disconnected.
			};

			std::string hostname;
			uint16_t port;
			std::vector<channel> channels {};

			std::shared_ptr<std::thread> server_thread;
			std::mutex status_mutex;
			stage status = unconnected;

			server(irc &parent, std::string hostname, int port): parent(parent), hostname(hostname), port(port) {}
			server(irc &parent, std::string hostname): server(parent, hostname, irc::default_port) {}

			void quote(const std::string &);

			operator std::string() const;
			bool start();
	};
}

#endif
