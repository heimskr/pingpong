#ifndef COMMANDS_PRIVMSG_H_
#define COMMANDS_PRIVMSG_H_

#include <string>

#include "pingpong/core/local.h"
#include "command.h"

namespace pingpong {
	class privmsg_command: public command, public local {
		public:
			std::string message;

			privmsg_command(server *serv_, std::string where_, std::string message_);

			privmsg_command(std::shared_ptr<channel> chan, std::string message_):
				privmsg_command(chan->serv, chan->name, message_) {}

			privmsg_command(std::shared_ptr<user> user, std::string message_):
				privmsg_command(user->serv, user->name, message_) {}

			operator std::string() const override;
			virtual bool send() override;
	};
}

#endif
