#ifndef COMMANDS_PONG_H_
#define COMMANDS_PONG_H_

#include "Command.h"

namespace PingPong {
	class PongCommand: public Command {
		public:
			std::string token;

			PongCommand(Server *server_, const std::string &token_): Command(server_), token(token_) {}
			operator std::string() const override;
			bool isSilent() const override { return true; }
	};
}

#endif
