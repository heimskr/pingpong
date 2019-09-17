#ifndef COMMANDS_PRIVMSG_H_
#define COMMANDS_PRIVMSG_H_

#include <string>

#include "command.h"

namespace pingpong {
	class privmsg_command: public command {
		public:
			std::string destination, message;

			privmsg_command(server *serv_, std::string destination_, std::string message_);

			privmsg_command(std::shared_ptr<channel> destination_, std::string message_):
				privmsg_command(destination_->serv, destination_->name, message_) {}

			privmsg_command(std::shared_ptr<user> destination_, std::string message_):
				privmsg_command(destination_->serv, destination_->name, message_) {}

			bool is_user() const;
			bool is_channel() const;

			std::shared_ptr<user> get_user() const;
			std::shared_ptr<channel> get_channel() const;

			operator std::string() const override;
			virtual void send() override;
	};
}

#endif
