#ifndef PINGPONG_COMMANDS_WHOIS_H_
#define PINGPONG_COMMANDS_WHOIS_H_

#include "pingpong/commands/command.h"

namespace pingpong {
	class user;

	class whois_command: public command {
		public:
			std::string whom;

			whois_command(server *serv_, const std::string &whom_): command(serv_), whom(whom_) {}
			whois_command(server *serv_, std::shared_ptr<user> whom_);

			operator std::string() const override;
	};
}

#endif
