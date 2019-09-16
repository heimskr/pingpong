#ifndef PINGPONG_CORE_SERVER_H_
#define PINGPONG_CORE_SERVER_H_

#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "core/ppdefs.h"
#include "core/channel.h"
#include "core/irc.h"

#include "messages/line.h"
#include "messages/message.h"

#include "net/sock.h"
#include "net/socket_buffer.h"

namespace pingpong {
	class message;

	class server {
		private:
			std::shared_ptr<net::sock> sock;
			std::shared_ptr<net::socket_buffer> buffer;
			std::shared_ptr<std::iostream> stream;
			irc *parent;
			std::string nick;

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
			int port;
			std::list<channel_ptr> channels {};
			std::list<user_ptr>    users    {};
			message_ptr last_message;

			std::thread worker;
			std::mutex status_mux;
			stage status = unconnected;

			server(irc *parent_, std::string hostname_, int port_ = irc::default_port):
				parent(parent_), hostname(hostname_), port(port_) {}

			~server();

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

			/** Returns whether the server knows a user to be in any of its channels. */
			bool has_user(user_ptr) const;

			/** Returns whether the server knows a user to be in any of its channels. */
			bool has_user(const std::string &) const;

			/** Retrieves a channel pointer by name. */
			channel_ptr get_channel(const std::string &, bool create = false);

			/** Retrieves a user pointer by name. */
			user_ptr get_user(const std::string &, bool create = false);

			/** Renames a user. */
			void rename_user(const std::string &old_nick, const std::string &new_nick);

			/** Retrieves the pointer for the user of the client. */
			user_ptr get_self();

			/** Returns the parent irc instance. */
			irc * get_parent() { return parent; }

			/** Returns a string representing the hostname and port (if not the default port) of the connection. */
			operator std::string() const;

			/** Connects to the server. */
			bool start();
	};
}

#endif
