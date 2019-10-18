#ifndef PINGPONG_CORE_SERVER_H_
#define PINGPONG_CORE_SERVER_H_

#include <condition_variable>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_set>

#include "pingpong/core/defs.h"
#include "pingpong/core/channel.h"
#include "pingpong/core/features.h"
#include "pingpong/core/irc.h"

#include "pingpong/messages/line.h"
#include "pingpong/messages/message.h"

#include "pingpong/net/sock.h"
#include "pingpong/net/socket_buffer.h"

namespace spjalla { class client; }

namespace pingpong {
	class message;

	class server {
		friend class spjalla::client;

		private:
			std::shared_ptr<net::sock> sock;
			std::shared_ptr<net::socket_buffer> buffer;
			std::shared_ptr<std::iostream> stream;
			irc *parent;
			std::string nick;

			/** Contains features requested and acknowledged by the server, not features supported by Spjalla. */
			std::unordered_set<features::type> enabled_features;

			/** Contains features supported by the server, not features supported by Spjalla. */
			std::unordered_set<features::type> supported_features;

			std::condition_variable death;
			std::mutex death_mutex, getline_mutex;

			void work_read();
			void work_reap();
			void handle_line(const pingpong::line &);
			void negotiate_capabilities();

			/** The number of capabilities we've requested that we're waiting for the server to respond to. */
			size_t caps_requested = 0;

		public:
			enum class stage {
				// Connecting to an IRC server occurs in multiple stages.
				unconnected, // At first, a socket hasn't even been connected yet.
				capneg,      // Once the socket is connected, you need to negotiate IRCv3 capabilities.
				setuser,     // After capabilities are negotiated, you need to send a USER message.
				setnick,     // After the USER message is sent, you need to declare your nickname.
				ready,       // After the nickname is successfully declared, the connection is ready.
				dead         // After the server has disconnected.
			};

			std::string id;
			std::string hostname;
			int port;
			std::list<std::shared_ptr<channel>> channels {};
			std::list<std::shared_ptr<user>>    users    {};
			std::shared_ptr<message> last_message;

			std::thread worker, reaper;
			std::recursive_mutex status_mutex;
			stage status = stage::unconnected;

			/** A map of the names of all servers in the network to their descriptions. */
			std::map<std::string, std::string> server_descriptions;

			server(irc *parent_, const std::string &id_, const std::string &hostname_, int port_ = irc::default_port);

			server(irc *parent_, const std::string &hostname_, int port_ = irc::default_port):
				server(parent_, parent_->create_id(hostname_), hostname_, port_) {}

			/** Sends a raw string to the server. */
			void quote(const std::string &);

			/** Requests a nickname change. */
			void set_nick(const std::string &, bool immediate = false);

			/** Returns the current nickname. */
			const std::string & get_nick() const { return nick; }

			/** Returns the server's status. */
			stage get_status();

			/** Sets the server's status. */
			void set_status(stage);

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

			/** Quits the server. */
			void quit(const std::string &message = "");

			/** Connects to the server. */
			bool start();

			/** Disconnects the server. */
			void kill();

			/** Starts the process of removing a server. */
			void reap();

			/** If the server's not already counted as dead, set its status to dead and dispatch an event. */
			void set_dead();

			/** Returns a string representing the hostname and port (if not the default port) of the connection. */
			operator std::string() const;

			/** Locks the mutex that protects the server status. */
			std::unique_lock<std::recursive_mutex> lock_status();

			/** Marks a feature as active. */
			void add_feature(features::type);

			/** Marks a feature as supported by the server. */
			void support_feature(features::type);

			/** Marks all features in a string as supported. */
			void support_features(const std::string &);

			/** Returns the set of features that the server supports. */
			const std::unordered_set<features::type> & get_supported_features() const { return supported_features; }

			/** Returns whether a given feature has is enabled. */
			bool feature_enabled(features::type) const;

			/** Called when a request for capabilities has been sent. */
			void sent_cap_req(size_t count) { caps_requested += count; }

			/** Called when the server responds with an ACK or NAK to a CAP REQ. */
			void cap_answered(size_t count);

			/** Returns the number of capability requests that the server hasn't yet responded to. */
			size_t get_caps_requested() const { return caps_requested; }

			/** Places the names of all joined channels into a container, starting at a given iterator. */
			template <typename Iter>
			void channel_names(Iter begin) const {
				std::transform(channels.begin(), channels.end(), begin, [](auto chan) { return chan->name; });
			}
	};
}

#endif
