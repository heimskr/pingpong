#ifndef PINGPONG_CORE_SERVER_H_
#define PINGPONG_CORE_SERVER_H_

#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "pingpong/core/defs.h"
#include "pingpong/core/channel.h"
#include "pingpong/core/irc.h"

#include "pingpong/messages/line.h"
#include "pingpong/messages/message.h"

#include "pingpong/net/sock.h"
#include "pingpong/net/socket_buffer.h"

namespace pingpong {
	class message;

	class server {
		private:
			std::shared_ptr<net::sock> sock;
			std::shared_ptr<net::socket_buffer> buffer;
			std::shared_ptr<std::iostream> stream;
			irc *parent;
			std::string nick;

			void work();
			void handle_line(const pingpong::line &);

		public:
			enum class stage {
				// Connecting to an IRC server occurs in multiple stages.
				unconnected, // At first, a socket hasn't even been connected yet.
				setuser,     // Once the socket is connected, you need to send a USER message.
				setnick,     // After the USER message is sent, you need to declare your nickname.
				ready,       // After the nickname has been successfully declared, the connection is ready.
				dead         // After the server has disconnected.
			};

			std::string id;
			std::string hostname;
			int port;
			std::list<std::shared_ptr<channel>> channels {};
			std::list<std::shared_ptr<user>>    users    {};
			std::shared_ptr<message> last_message;

			std::thread worker;
			std::mutex status_mux;
			stage status = stage::unconnected;

			server(irc *parent_, const std::string &id_, const std::string &hostname_, int port_ = irc::default_port):
				parent(parent_), id(id_), hostname(hostname_), port(port_) {}

			server(irc *parent_, const std::string &hostname_, int port_ = irc::default_port):
				server(parent_, parent_->create_id(hostname_), hostname_, port_) {}

			~server();

			/** Sends a raw string to the server. */
			void quote(const std::string &);

			/** Requests a nickname change. */
			void set_nick(const std::string &);

			/** Returns the current nickname. */
			const std::string & get_nick() const { return nick; }

			/** Returns the server's status. */
			stage get_status() const { return status; }

			/** Stringifies the server's status. */
			std::string status_string() const;

			/** Adds a channel. Returns false if the channel was already present in the server. */
			bool add_channel(const std::string &);

			/** Removes a channel from the server if possible and returns whether it was successfully removed. */
			bool remove_channel(const std::shared_ptr<channel> &);

			/** Removes a channel from the server if possible and returns whether it was successfully removed. */
			bool remove_channel(const std::string &);

			/** Removes a user from the server if possible ande returns whether they were successfully removed. */
			bool remove_user(const std::shared_ptr<user> &);

			/** Removes a user from the server if possible ande returns whether they were successfully removed. */
			bool remove_user(const std::string &);

			/** Returns whether the user is in a given channel. */
			bool has_channel(const std::string &) const;

			/** Returns whether the server knows a user to be in any of its channels. */
			bool has_user(std::shared_ptr<user>) const;

			/** Returns whether the server knows a user to be in any of its channels. */
			bool has_user(const std::string &) const;

			/** Retrieves a channel pointer by name. */
			std::shared_ptr<channel> get_channel(const std::string &, bool create = false);

			/** Retrieves a user pointer by name. */
			std::shared_ptr<user> get_user(const std::string &, bool create = false);

			/** Retrieves a user pointer by mask. */
			std::shared_ptr<user> get_user(const mask &, bool create = false);

			/** Renames a user. */
			void rename_user(const std::string &old_nick, const std::string &new_nick);

			/** Sorts the list of channels by name. */
			void sort_channels();

			/** Returns whether the server is in a valid state. */
			bool is_active() const;

			/** Retrieves the pointer for the user of the client. */
			std::shared_ptr<user> get_self();

			/** Returns the parent irc instance. */
			irc * get_parent() { return parent; }

			/** Connects to the server. */
			bool start();

			/** Disconnects the server. */
			void cleanup();

			/** Returns a string representing the hostname and port (if not the default port) of the connection. */
			operator std::string() const;
	};
}

#endif
