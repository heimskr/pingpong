#ifndef PINGPONG_COMMANDS_WHOIS_H_
#define PINGPONG_COMMANDS_WHOIS_H_

#include "pingpong/commands/Command.h"

namespace PingPong {
	class User;

	class WhoisCommand: public Command {
		public:
			std::string whom;

			WhoisCommand(Server *server_, const std::string &whom_): Command(server_), whom(whom_) {}
			WhoisCommand(Server *server_, std::shared_ptr<User> whom_);

			operator std::string() const override;
	};
}

#endif
