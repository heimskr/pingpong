#ifndef COMMANDS_USER_H_
#define COMMANDS_USER_H_

#include "command.h"

namespace pingpong {
	class user_command: public command {
		public:
			std::string username;
			std::string realname;

			user_command(serv_ptr serv_, std::string username_, std::string realname_):
				command(serv_), username(username_), realname(realname_) {}

			virtual std::string to_string() const;
	};
}

#endif
