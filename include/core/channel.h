#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <ostream>
#include <string>

#include "core/user.h"
#include "core/defs.h"
#include "core/server.h"

namespace pingpong {
	class user;

	class channel {
		public:
			std::string name;
			server_ptr serv;
			std::vector<pingpong::user> users;

			channel(std::string, server_ptr);
			channel(std::string);
			bool is_user() const;

			operator std::string() const { return name; }
			bool operator==(const std::string &) const;
			bool operator==(const channel &) const;
			bool operator<(const channel &) const;
			bool has_server() const;

			friend std::ostream & operator<<(std::ostream &os, const channel &chan);
	};
}

#endif
