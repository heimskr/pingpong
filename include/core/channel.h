#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <map>
#include <memory>
#include <ostream>
#include <string>

#include "core/user.h"
#include "core/defs.h"
#include "core/server.h"

namespace pingpong {
	class user;

	class channel {
		public:
			enum class visibility: char {pub='=', priv='*', secret='@'};

			std::string name;
			server_ptr serv;
			std::map<std::string, user_ptr> users;
			std::map<user_ptr, hat> hats;

			channel(std::string, server_ptr);
			channel(std::string);

			bool has_server() const;
			bool rename_user(const std::string &, const std::string &);

			operator std::string() const { return name; }
			user_ptr operator[](const std::string &);
			bool operator==(const std::string &) const;
			bool operator==(const channel &) const;
			bool operator<(const channel &) const;

			friend std::ostream & operator<<(std::ostream &os, const channel &chan);
	};
}

#endif
