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
			irc *parent;
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

			std::shared_ptr<std::thread> worker;
			std::mutex status_mux;
			stage status = unconnected;

			server(irc *parent_, std::string hostname_, int port_):
				parent(parent_), hostname(hostname_), port(port_) {}
			server(irc *parent_, std::string hostname_):
				server(parent_, hostname_, irc::default_port) {}

			/** Adds a channel. */
			server & operator+=(const std::string &);

			/** Removes a channel. */
			server & operator-=(const std::string &);

			/** Sends a raw string to the server. */
			void quote(const std::string &);

			/** Requests a nickname change. */
			void set_nick(const std::string &);

			/** Returns the current nickname. */
			const std::string & get_nick() const { return nick; }

			/** Returns whether the user is in a given channel. */
			bool has_channel(const std::string &) const;

			/** Retrieves a channel pointer by name. */
			channel_ptr get_channel(const std::string &) const;

			/** Retrieves a user pointer by name. */
			user_ptr get_user(const std::string &, bool create = true);

			/** Renames a user. */
			void rename_user(const std::string &old_nick, const std::string &new_nick);

			/** Returns the parent irc instance. */
			irc * get_parent() { return parent; }

			/** Returns a string representing the hostname and port (if not the default port) of the connection. */
			operator std::string() const;

			/** Connects to the server. */
			bool start();
	};
}

#endif
