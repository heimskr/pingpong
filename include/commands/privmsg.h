#ifndef COMMANDS_PRIVMSG_H_
#define COMMANDS_PRIVMSG_H_

#include <string>

#include "command.h"

namespace pingpong {
	class privmsg_command: public command {
		public:
			std::shared_ptr<channel> destination;
			std::string message;

			privmsg_command(server *serv_, std::shared_ptr<channel> destination_, std::string message_):
				command(serv_), destination(destination_), message(message_) {}

			privmsg_command(server *serv_, std::string destination_, std::string message_):
				privmsg_command(serv_->get_channel(destination_), message_) {}

			privmsg_command(std::shared_ptr<channel> destination_, std::string message_):
				privmsg_command(destination_->serv, destination_, message_) {}

			operator std::string() const override;
			virtual void send() override;
	};
}

#endif
