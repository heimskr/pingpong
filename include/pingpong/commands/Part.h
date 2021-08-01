#ifndef PINGPONG_COMMANDS_PART_H_
#define PINGPONG_COMMANDS_PART_H_

#include "pingpong/core/Channel.h"
#include "Command.h"

namespace PingPong {
	class PartCommand: public Command {
		public:
			std::string channels, reason;

			PartCommand(Server *server_, const std::string &channels_, const std::string &reason_ = ""):
				Command(server_), channels(channels_), reason(reason_) {}

			PartCommand(Channel *channel, const std::string &reason_ = ""):
				PartCommand(channel->server, channel->name, reason_) {}

			PartCommand(std::shared_ptr<Channel> channel, const std::string &reason_ = ""):
				PartCommand(channel.get(), reason_) {}

			operator std::string() const override;
	};
}

#endif
