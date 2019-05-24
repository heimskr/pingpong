#ifndef CORE_SERVER_H_
#define CORE_SERVER_H_

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <map>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"

#include "core/defs.h"
#include "core/channel.h"
#include "core/irc.h"
#include "messages/line.h"
#include "messages/message.h"

using Poco::Net::SocketAddress, Poco::Net::StreamSocket, Poco::Net::SocketStream;

namespace pingpong {
	class message;

	class server {
		private:
			StreamSocket socket;
			std::shared_ptr<SocketStream> stream;
			std::shared_ptr<irc> parent;
			std::string nick;

			void cleanup(std::unique_lock<std::mutex> &);
			void cleanup();
			void work();
			void handle_line(const pingpong::line &);

		public:
			enum stage {
				// Connecting to an IRC server occurs in multiple stages.
				unconnected, // At first, a socket hasn't even been connected yet.
				setuser,     // Once the socket is connected, you need to send a USER message.
				setnick,     // After the USER message is sent, you need to declare your nickname.
				registered,  // After the nickname has been successfully declared, the connection is ready.
				dead         // After the server has disconnected.
			};

			std::string hostname;
			uint16_t port;
			std::map<std::string, channel_ptr> channels {};
			std::map<std::string, user_ptr> users{};
			channel_ptr active_channel = nullptr;
			message_ptr last_message;

			std::shared_ptr<std::thread> server_thread;
			std::mutex status_mux;
			stage status = unconnected;

			server(std::shared_ptr<irc> parent, std::string hostname, int port):
				parent(parent), hostname(hostname), port(port) {}
			server(std::shared_ptr<irc> parent, std::string hostname):
				server(parent, hostname, irc::default_port) {}

			// Adds a channel.
			server & operator+=(const std::string &);

			// Removes a channel.
			server & operator-=(const std::string &);

			void quote(const std::string &, bool = false);
			void set_nick(const std::string &);
			const std::string & get_nick() const;
			bool has_channel(const std::string &) const;
			channel_ptr get_channel(const std::string &) const;
			user_ptr get_user(const std::string &, bool = true);
			void rename_user(const std::string &, const std::string &);

			operator std::string() const;
			bool start();
	};
}

#endif
