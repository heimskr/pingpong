#include <string>

#include "privmsg.h"

namespace pingpong {
	privmsg_command::privmsg_command(channel destination_, std::string message_):
		command(destination_.serv), destination(destination_), message(message_) {}

	privmsg_command::privmsg_command(serv_ptr serv_, std::string destination_, std::string message_):
		privmsg_command(channel(destination_, serv_), message_) {}

	std::string privmsg_command::to_string() const {
		return "PRIVMSG " + destination.name + " :" + message;
	}
}
