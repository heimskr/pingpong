#ifndef PINGPONG_CORE_CHANNEL_H_
#define PINGPONG_CORE_CHANNEL_H_

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <unordered_set>

#include "pingpong/core/defs.h"
#include "pingpong/core/hats.h"
#include "pingpong/core/moded.h"
#include "pingpong/core/topicset.h"
#include "pingpong/core/user.h"

namespace pingpong {
	class user;
	class server;

	class channel: public moded {
		private:
			std::mutex users_mutex;

		public:
			enum class visibility: char {pub='=', priv='*', secret='@'};

			std::string name;
			server *serv;
			std::list<std::shared_ptr<user>> users;
			std::map<std::shared_ptr<user>, hat_set> hats;
			topicset topic;

			channel(std::string, server *);
			channel(std::string);

			/** Returns whether the channel is associated with a server (it really should be...). */
			bool has_server() const;

			/** Adds a user to the channel. */
			bool add_user(std::shared_ptr<user>);

			/** Removes a user from the channel if possible and returns whether the user was successfully removed. */
			bool remove_user(std::shared_ptr<user>);

			/** Renames a user in the channel. */
			bool rename_user(const std::string &, const std::string &);

			/** Returns whether a particular user is in the channel. */
			bool has_user(std::shared_ptr<user>) const;

			/** Determines whether there's any user in the channel with a given nick. */
			bool has_user(const std::string &) const;

			/** Updates the hat information for a user. Returns true if any existing data was overwritten. */
			bool set_hats(std::shared_ptr<user>, const hat_set &);

			/** Returns the hat corresponding to a user if it's known, or the default hat otherwise. */
			hat_set & get_hats(std::shared_ptr<user> user);

			/** Should be called whenever a user speaks. Reorders the list of users to put the user at the front.
			 *  Returns whether the user was found and sent to the front. */
			bool send_to_front(std::shared_ptr<user>);

			/** Should be called whenever a user speaks. Reorders the list of users to put the user at the front.
			 *  Returns whether the user was found and sent to the front. */
			bool send_to_front(const std::string &);

			operator std::string() const;
			std::shared_ptr<user> operator[](const std::string &);
			bool operator==(const std::string &) const;
			bool operator!=(const std::string &) const;
			bool operator==(const channel &) const;
			bool operator!=(const channel &) const;
			bool operator<(const channel &) const;

			/** Returns a lock on the users list. */
			std::unique_lock<std::mutex> lock_users() { return std::unique_lock(users_mutex); }

			friend std::ostream & operator<<(std::ostream &os, const channel &chan);
	};
}

#endif
