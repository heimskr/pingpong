#ifndef PINGPONG_COMMANDS_MODE_H_
#define PINGPONG_COMMANDS_MODE_H_

#include "pingpong/core/Local.h"
#include "Command.h"

namespace PingPong {
	class ModeCommand: public Command, public Local {
		public:
			std::string flags, extra;

			ModeCommand(const std::string &where_, Server *server_, const std::string &flags_ = "",
			const std::string &extra_ = ""):
				Command(server_), Local(where_), flags(flags_), extra(extra_) {}

			ModeCommand(std::shared_ptr<User> who, const std::string &flags_ = "", const std::string &extra_ = ""):
				ModeCommand(who->name, who->server, flags_, extra_) {}

			ModeCommand(std::shared_ptr<Channel> channel, const std::string &flags_ = "",
			const std::string &extra_ = ""):
				ModeCommand(channel->name, channel->server, flags_, extra_) {}

			operator std::string() const override;
	};
}

#endif
