#ifndef PINGPONG_CORE_CHANNEL_H_
#define PINGPONG_CORE_CHANNEL_H_

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>

#include "core/user.h"
#include "core/ppdefs.h"
#include "core/server.h"

namespace pingpong {
	class user;

	class channel {
		public:
			enum class visibility: char {pub='=', priv='*', secret='@'};

			std::string name;
			server *serv;
			std::list<std::shared_ptr<user>> users;
			std::map<std::shared_ptr<user>, hat> hats;

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

			/** Returns the hat corresponding to a user if it's known, or the default hat otherwise. */
			hat get_hat(std::shared_ptr<user> user) const;

			operator std::string() const;
			std::shared_ptr<user> operator[](const std::string &);
			bool operator==(const std::string &) const;
			bool operator!=(const std::string &) const;
			bool operator==(const channel &) const;
			bool operator!=(const channel &) const;
			bool operator<(const channel &) const;

			friend std::ostream & operator<<(std::ostream &os, const channel &chan);
	};
}

#endif
