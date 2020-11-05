#ifndef PINGPONG_COMMANDS_PRIVMSG_H_
#define PINGPONG_COMMANDS_PRIVMSG_H_

#include <string>

#include "pingpong/core/Local.h"
#include "Command.h"

namespace PingPong {
	struct PrivmsgCommand: public Command, public Local {
		std::string message;

		PrivmsgCommand(Server *, std::string where_, std::string message_);

		PrivmsgCommand(std::shared_ptr<Channel> channel, std::string message_):
			PrivmsgCommand(channel->server, channel->name, message_) {}

		PrivmsgCommand(std::shared_ptr<User> user, std::string message_):
			PrivmsgCommand(user->server, user->name, message_) {}

		operator std::string() const override;
		virtual bool send() override;
	};
}

#endif
