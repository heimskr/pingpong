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

#include "pingpong/core/Defs.h"
#include "pingpong/core/Channel.h"
#include "pingpong/core/Features.h"
#include "pingpong/core/IRC.h"

#include "pingpong/messages/Line.h"
#include "pingpong/messages/Message.h"

#include "pingpong/net/Sock.h"
#include "pingpong/net/SocketBuffer.h"

namespace Spjalla { class Client; }

namespace PingPong {
	class Message;

	class Server {
		friend class Spjalla::Client;

		private:
			std::shared_ptr<Net::Sock> sock;
			std::shared_ptr<Net::SocketBuffer> buffer;
			std::shared_ptr<std::iostream> stream;
			IRC *parent;
			std::string nick;

			/** Contains features requested and acknowledged by the server, not features supported by Spjalla. */
			std::unordered_set<Features::Type> enabledFeatures;

			/** Contains features supported by the server, not features supported by Spjalla. */
			std::unordered_set<Features::Type> supportedFeatures;

			std::condition_variable death;
			std::mutex deathMutex, getlineMutex;

			void workRead();
			void workReap();
			void handleLine(const PingPong::Line &);
			void negotiateCapabilities();

			/** The number of capabilities we've requested that we're waiting for the server to respond to. */
			size_t capsRequested = 0;

		public:
			enum class Stage {
				// Connecting to an IRC server occurs in multiple stages.
				Unconnected, // At first, a socket hasn't even been connected yet.
				CapNeg,      // Once the socket is connected, you need to negotiate IRCv3 capabilities.
				SetUser,     // After capabilities are negotiated, you need to send a USER message.
				SetNick,     // After the USER message is sent, you need to declare your nickname.
				Ready,       // After the nickname is successfully declared, the connection is ready.
				Dead         // After the server has disconnected.
			};

			std::string id;
			std::string hostname;
			int port;
			std::list<std::shared_ptr<Channel>> channels {};
			std::list<std::shared_ptr<User>>    users    {};
			std::shared_ptr<Message> lastMessage;

			std::thread worker, reaper;
			std::recursive_mutex statusMutex;
			Stage status = Stage::Unconnected;

			/** A map of the names of all servers in the network to their descriptions. */
			std::map<std::string, std::string> serverDescriptions;

			Server(IRC *parent_, const std::string &id_, const std::string &hostname_, int port_ = IRC::defaultPort);

			Server(IRC *parent_, const std::string &hostname_, int port_ = IRC::defaultPort):
				Server(parent_, parent_->createID(hostname_), hostname_, port_) {}

			/** Sends a raw string to the server. */
			void quote(const std::string &);

			/** Requests a nickname change. */
			void setNick(const std::string &, bool immediate = false);

			/** Returns the current nickname. */
			const std::string & getNick() const { return nick; }

			/** Returns the server's status. */
			Stage getStatus();

			/** Sets the server's status. */
			void setStatus(Stage);

			/** Stringifies the server's status. */
			std::string statusString() const;

			/** Adds a channel. Returns false if the channel was already present in the server. */
			bool addChannel(const std::string &);

			/** Removes a channel from the server if possible and returns whether it was successfully removed. */
			bool removeChannel(const std::shared_ptr<Channel> &);

			/** Removes a channel from the server if possible and returns whether it was successfully removed. */
			bool removeChannel(const std::string &);

			/** Removes a user from the server if possible ande returns whether they were successfully removed. */
			bool removeUser(const std::shared_ptr<User> &);

			/** Removes a user from the server if possible ande returns whether they were successfully removed. */
			bool removeUser(const std::string &);

			/** Returns whether the user is in a given channel. */
			bool hasChannel(const std::string &) const;

			/** Returns whether the server knows a user to be in any of its channels. */
			bool hasUser(std::shared_ptr<User>) const;

			/** Returns whether the server knows a user to be in any of its channels. */
			bool hasUser(const std::string &) const;

			/** Retrieves a channel pointer by name. */
			std::shared_ptr<Channel> getChannel(const std::string &, bool create = false);

			/** Retrieves a user pointer by name. */
			std::shared_ptr<User> getUser(const std::string &, bool create = false, bool update_case = false);

			/** Retrieves a user pointer by mask. */
			std::shared_ptr<User> getUser(const Mask &, bool create = false, bool update_case = false);

			/** Renames a user. */
			void renameUser(const std::string &old_nick, const std::string &new_nick);

			/** Sorts the list of channels by name. */
			void sortChannels();

			/** Returns whether the server is in a valid state. */
			bool isActive() const;

			/** Retrieves the pointer for the user of the client. */
			std::shared_ptr<User> getSelf();

			/** Returns the parent irc instance. */
			IRC * getParent() { return parent; }

			/** Quits the server. */
			void quit(const std::string &message = "");

			/** Connects to the server. */
			bool start();

			/** Disconnects the server. */
			void kill();

			/** Starts the process of removing a server. */
			void reap();

			/** If the server's not already counted as dead, set its status to dead and dispatch an event. */
			void setDead();

			/** Returns a string representing the hostname and port (if not the default port) of the connection. */
			operator std::string() const;

			/** Locks the mutex that protects the server status. */
			std::unique_lock<std::recursive_mutex> lockStatus();

			/** Marks a feature as active. */
			void addFeature(Features::Type);

			/** Marks a feature as supported by the server. */
			void supportFeature(Features::Type);

			/** Marks all features in a string as supported. */
			void supportFeatures(const std::string &);

			/** Returns the set of features that the server supports. */
			const std::unordered_set<Features::Type> & getSupportedFeatures() const { return supportedFeatures; }

			/** Returns whether a given feature has is enabled. */
			bool featureEnabled(Features::Type) const;

			/** Called when a request for capabilities has been sent. */
			void sentCapReq(size_t count) { capsRequested += count; }

			/** Called when the server responds with an ACK or NAK to a CAP REQ. */
			void capAnswered(size_t count);

			/** Returns the number of capability requests that the server hasn't yet responded to. */
			size_t getCapsRequested() const { return capsRequested; }

			/** Places the names of all joined channels into a container, starting at a given iterator. */
			template <typename Iter>
			void channelNames(Iter begin) const {
				std::transform(channels.begin(), channels.end(), begin, [](auto chan) { return chan->name; });
			}
	};
}

#endif
