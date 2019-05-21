#ifndef COMMANDS_MSG_H_
#define COMMANDS_MSG_H_

#include <string>

#include "core/channel.h"
#include "command.h"

namespace pingpong {
	class msg_command: public command {
		public:
			using command::command;

			channel destination;
			std::string text;

			msg_command(server_ptr serv, std::string dest, std::string text_):
				command(serv), destination(channel(dest, serv)), text(text_) {}
			
			msg_command(channel dest, std::string text_): command(dest.serv), destination(dest), text(text_) {}

			operator std::string() const override;
	};
}

#endif
