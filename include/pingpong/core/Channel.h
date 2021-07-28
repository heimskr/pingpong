#pragma once

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <unordered_set>

#include "pingpong/core/Defs.h"
#include "pingpong/core/Hats.h"
#include "pingpong/core/Moded.h"
#include "pingpong/core/TopicSet.h"
#include "pingpong/core/User.h"

namespace PingPong {
	class User;
	class Server;

	class Channel: public Moded {
		private:
			std::mutex usersMutex;

		public:
			enum class Visibility: char {Public='=', Private='*', Secret='@'};

			std::string name;
			Server *server;
			std::list<std::shared_ptr<User>> users;
			std::map<std::shared_ptr<User>, HatSet> hats;
			TopicSet topic;

			Channel(const std::string &, Server * = nullptr);

			/** Returns whether the channel is associated with a server (it really should be...). */
			bool hasServer() const;

			/** Adds a user to the channel. */
			bool addUser(std::shared_ptr<User>);

			/** Removes a user from the channel if possible and returns whether the user was successfully removed. */
			bool removeUser(std::shared_ptr<User>);

			/** Renames a user in the channel. */
			bool renameUser(const std::string &, const std::string &);

			/** Returns whether a particular user is in the channel. */
			bool hasUser(std::shared_ptr<User>) const;

			/** Determines whether there's any user in the channel with a given nick. */
			bool hasUser(const std::string &) const;

			/** Updates the hat information for a user. Returns true if any existing data was overwritten. */
			bool setHats(std::shared_ptr<User>, const HatSet &);

			/** Returns the hat corresponding to a user if it's known, or the default hat otherwise. */
			HatSet & getHats(std::shared_ptr<User> user);

			/** Should be called whenever a user speaks. Reorders the list of users to put the user at the front.
			 *  Returns whether the user was found and sent to the front. */
			bool sendToFront(std::shared_ptr<User>);

			/** Should be called whenever a user speaks. Reorders the list of users to put the user at the front.
			 *  Returns whether the user was found and sent to the front. */
			bool sendToFront(const std::string &);

			/** Sorts the user list case-insensitively by name. Ignores hats. */
			void sortUsers();

			operator std::string() const;
			std::shared_ptr<User> operator[](const std::string &);
			bool operator==(const std::string &) const;
			bool operator!=(const std::string &) const;
			bool operator==(const Channel &) const;
			bool operator!=(const Channel &) const;
			bool operator<(const Channel &) const;

			/** Returns a lock on the users list. */
			std::unique_lock<std::mutex> lockUsers() { return std::unique_lock(usersMutex); }

			friend std::ostream & operator<<(std::ostream &, const Channel &);
	};
}
