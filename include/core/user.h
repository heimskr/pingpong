#ifndef PINGPONG_CORE_USER_H_
#define PINGPONG_CORE_USER_H_

#include <memory>
#include <set>
#include <string>
#include <unordered_set>

#include "core/ppdefs.h"
#include "core/pputil.h"
#include "core/server.h"

namespace pingpong {
	class user {
		public:
			std::string name;

			pingpong::server *serv = nullptr;
			std::set<std::weak_ptr<channel>, weakptr_compare<channel>> channels = {};

			user(const std::string &name_, server *serv_): name(name_), serv(serv_) {}

			void rename(const std::string &);
			operator std::string() const { return name; }
			
			template <typename T>
			user & operator+=(T chan) {
				channels.insert(std::weak_ptr<channel>(chan));
				return *this;
			}

			friend std::ostream & operator<<(std::ostream &os, const user &who);

			static hat get_hat(char);
			static hat get_hat(const std::string &);
			bool is_self() const;

			bool operator==(const user &) const;
	};
}

#endif
