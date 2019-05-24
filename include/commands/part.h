#ifndef COMMANDS_PART_H_
#define COMMANDS_PART_H_

#include <string>

#include "core/channel.h"
#include "commands/command.h"

namespace pingpong {
	class part_command: public command {
		public:
			using command::command;

			channel_ptr chan;
			std::string reason;

			part_command(server_ptr serv, const channel_ptr &chan_, const std::string &reason_):
				command(serv), chan(chan_), reason(reason_) {}
			part_command(server_ptr serv, const channel_ptr &chan_): command(serv), chan(chan_), reason("") {}

			operator std::string() const override;
	};
}

#endif
