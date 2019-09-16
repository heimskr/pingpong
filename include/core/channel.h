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
			server_ptr serv;
			std::list<user_ptr> users;
			std::map<user_ptr, hat> hats;

			channel(std::string, server_ptr);
			channel(std::string);

			/** Returns whether the channel is associated with a server (it really should be...). */
			bool has_server() const;

			/** Renames a user in the channel. */
			bool rename_user(const std::string &, const std::string &);

			/** Returns whether a particular user is in the channel. */
			bool has_user(user_ptr) const;

			/** Determines whether there's any user in the channel with a given nick. */
			bool has_user(const std::string &) const;

			/** Returns the hat corresponding to a user if it's known, or the default hat otherwise. */
			hat get_hat(user_ptr user) const;

			operator std::string() const;
			user_ptr operator[](const std::string &);
			channel & operator+=(user_ptr);
			channel & operator-=(user_ptr);
			bool operator==(const std::string &) const;
			bool operator!=(const std::string &) const;
			bool operator==(const channel &) const;
			bool operator!=(const channel &) const;
			bool operator<(const channel &) const;

			friend std::ostream & operator<<(std::ostream &os, const channel &chan);
	};
}

#endif
