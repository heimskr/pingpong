#ifndef COMMANDS_PONG_H_
#define COMMANDS_PONG_H_

#include "command.h"

namespace pingpong {
	class pong_command: public command {
		public:
			std::string token;

			pong_command(server *serv_, const std::string &token_): command(serv_), token(token_) {}
			operator std::string() const override;
			bool is_silent() const override { return true; }
	};
}

#endif
