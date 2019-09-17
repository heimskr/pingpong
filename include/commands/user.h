#ifndef COMMANDS_USER_H_
#define COMMANDS_USER_H_

#include "command.h"

namespace pingpong {
	class user_command: public command {
		public:
			std::string username, realname;

			user_command(server *serv_, const std::string &username_, const std::string &realname_):
				command(serv_), username(username_), realname(realname_) {}

			operator std::string() const override;
			void send() override;
	};
}

#endif
