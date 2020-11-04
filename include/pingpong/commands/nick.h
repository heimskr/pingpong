#ifndef COMMANDS_NICK_H_
#define COMMANDS_NICK_H_

#include "Command.h"

namespace PingPong {
	struct NickCommand: public Command {
		std::string nickname;

		NickCommand(Server *server_, const std::string &nickname_): Command(server_), nickname(nickname_) {}
		operator std::string() const override;
	};
}

#endif
