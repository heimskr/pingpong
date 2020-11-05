#ifndef COMMANDS_USER_H_
#define COMMANDS_USER_H_

#include "Command.h"

namespace PingPong {
	class UserCommand: public Command {
		public:
			std::string username, realname;

			UserCommand(Server *server_, const std::string &username_, const std::string &realname_):
				Command(server_), username(username_), realname(realname_) {}

			operator std::string() const override;
			virtual bool send() override;
	};
}

#endif
