#ifndef COMMANDS_NICK_H_
#define COMMANDS_NICK_H_

#include "command.h"

namespace pingpong {
	class nick_command: public command {
		public:
			std::string nickname;

			nick_command(server *serv_, const std::string &nickname_): command(serv_), nickname(nickname_) {}
			operator std::string() const override;
	};
}

#endif
