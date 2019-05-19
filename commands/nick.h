#ifndef COMMANDS_NICK_H_
#define COMMANDS_NICK_H_

#include "command.h"

namespace pingpong {
	class nick_command: public command {
		public:
			std::string nickname;

			nick_command(std::shared_ptr<server> serv_, std::string nickname_): command(serv_), nickname(nickname_) {}
			virtual std::string to_string() const;
	};
}

#endif
