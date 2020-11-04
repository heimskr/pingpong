#ifndef PINGPONG_COMMANDS_KICK_H_
#define PINGPONG_COMMANDS_KICK_H_

#include "Command.h"

namespace PingPong {
	class KickCommand: public Command {
		public:
			std::string channel, whom, reason;

			KickCommand(Server *server_, const std::string &channel_, const std::string &whom_,
			const std::string &reason_ = ""):
				Command(server_), channel(channel_), whom(whom_), reason(reason_) {}

			KickCommand(Server *server_, std::shared_ptr<Channel> channel_, const std::string &whom_,
			const std::string &reason_ = "");

			operator std::string() const override;
	};
}

#endif
