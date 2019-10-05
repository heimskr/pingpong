#ifndef COMMANDS_QUIT_H_
#define COMMANDS_QUIT_H_

#include <string>

#include "command.h"

namespace pingpong {
	class 
	quit_command: public command {
		public:
			std::string reason;

			quit_command(server *serv_, const std::string &reason_ = ""): command(serv_), reason(reason_) {}

			operator std::string() const override;
			virtual bool send() override;
	};
}

#endif
