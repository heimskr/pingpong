#ifndef COMMANDS_QUIT_H_
#define COMMANDS_QUIT_H_

#include <string>

#include "command.h"

namespace pingpong {
	class quit_command: public command {
		public:
			using command::command;

			std::string reason;

			quit_command(server_ptr serv, std::string reason_): command(serv), reason(reason_) {}
			quit_command(server_ptr serv): quit_command(serv, "") {}

			operator std::string() const override;
	};
}

#endif
