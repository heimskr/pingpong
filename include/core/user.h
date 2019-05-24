#ifndef CORE_USER_H_
#define CORE_USER_H_

#include <string>
#include <unordered_set>

#include "core/defs.h"
#include "core/server.h"

namespace pingpong {
	class user {
		public:
			std::string name;

			server_ptr serv = nullptr;
			std::unordered_set<channel_ptr> channels = {};

			user(const std::string &name_, server_ptr serv_): name(name_), serv(serv_) {}

			void rename(const std::string &);
			operator std::string() const { return name; }
			user & operator+=(channel_ptr);

			friend std::ostream & operator<<(std::ostream &os, const user &who);
			static hat get_hat(char);
			static hat get_hat(const std::string &);
	};
}

#endif
