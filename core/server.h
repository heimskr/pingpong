#ifndef CORE_SERVER_H_
#define CORE_SERVER_H_

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "channel.h"
#include "irc.h"

namespace pingpong {
	using serv_ptr = std::shared_ptr<server>;

	class server {
		private:
			void cleanup(std::unique_lock<std::mutex> &);
			void cleanup();

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
			int port = irc::default_port;
			std::vector<channel> channels {};

			std::shared_ptr<std::thread> server_thread;
			std::mutex status_mutex;
			stage status = unconnected;

			server(std::string hostname): hostname(hostname) {}
			server(std::string hostname, int port): hostname(hostname), port(port) {}

			void quote(const std::string &raw);

			std::string to_string() const;
			operator std::string() const { return to_string(); }
			bool start();
	};
}

#endif
