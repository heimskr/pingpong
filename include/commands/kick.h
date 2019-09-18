#ifndef PINGPONG_COMMANDS_KICK_H_
#define PINGPONG_COMMANDS_KICK_H_

#include "command.h"

namespace pingpong {
	class kick_command: public command {
		public:
			std::string chan, whom, reason;

			kick_command(server *serv_, std::string chan_, const std::string &whom_, const std::string &reason_ = ""):
				command(serv_), chan(chan_), whom(whom_), reason(reason_) {}

			kick_command(server *serv_, std::shared_ptr<channel> chan_, const std::string &whom_,
			const std::string &reason_ = ""):
				kick_command(serv_, chan_->name, whom_, reason_) {}

			operator std::string() const override;
	};
}

#endif
