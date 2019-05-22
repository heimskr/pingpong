#ifndef CORE_USER_H_
#define CORE_USER_H_

#include <string>

#include "core/server.h"

namespace pingpong {
	class user {
		private:
			std::string name;

		public:
			server_ptr serv;

			user(const std::string &name_, server_ptr serv_): name(name_), serv(serv_) {}

			void rename(const std::string &);
			operator std::string() const { return name; }

			friend std::ostream & operator<<(std::ostream &os, const user &who);
	};
}

#endif
