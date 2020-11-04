#ifndef COMMANDS_QUIT_H_
#define COMMANDS_QUIT_H_

#include <string>

#include "Command.h"

namespace PingPong {
	struct QuitCommand: public Command {
		std::string reason;

		QuitCommand(Server *server_, const std::string &reason_ = ""): Command(server_), reason(reason_) {}

		operator std::string() const override;
		virtual bool send() override;
	};
}

#endif
