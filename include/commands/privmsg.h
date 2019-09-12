#ifndef COMMANDS_PRIVMSG_H_
#define COMMANDS_PRIVMSG_H_

#include <string>

#include "command.h"

namespace pingpong {
	class privmsg_command: public command {
		public:
			channel_ptr destination;
			std::string message;

			privmsg_command(channel_ptr destination_, std::string message_):
				command(destination_->serv), destination(destination_), message(message_) {}

			privmsg_command(server_ptr serv_, std::string destination_, std::string message_):
				privmsg_command(serv_->get_channel(destination_), message_) {}

			operator std::string() const override;
	};
}

#endif
