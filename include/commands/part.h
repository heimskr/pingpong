#ifndef PINGPONG_COMMANDS_PART_H_
#define PINGPONG_COMMANDS_PART_H_

#include "core/channel.h"
#include "commands/command.h"

namespace pingpong {
	class part_command: public command {
		public:
			std::string chans, reason;

			part_command(server *serv, std::string chans_, const std::string &reason_ = ""):
				command(serv), chans(chans_), reason(reason_) {}

			part_command(server *serv, std::shared_ptr<channel> chan, const std::string &reason_ = ""):
				part_command(serv, chan->name, reason_) {}

			operator std::string() const override;
	};
}

#endif
