#include "pingpong/commands/Privmsg.h"

#include "pingpong/events/Error.h"
#include "pingpong/events/Privmsg.h"

namespace PingPong {
	PrivmsgCommand::PrivmsgCommand(Server *server_, std::string where_, std::string message_):
	Command(server_), Local(where_), message(message_) {
		if (where.empty())
			throw std::runtime_error("Destination is empty in PrivmsgCommand");
	}

	PrivmsgCommand::operator std::string() const {
		return "PRIVMSG " + where + " :" + message;
	}

	bool PrivmsgCommand::send() {
		if (Command::send()) {
			if (isChannel()) {
				std::shared_ptr<Channel> channel = getChannel(server);
				if (channel)
					Events::dispatch<PrivmsgEvent>(server->getSelf(), channel, message);
				else
					Events::dispatch<ErrorEvent>("Can't send message: channel is null", false);
			} else {
				Events::dispatch<PrivmsgEvent>(server->getSelf(), getUser(server, false), message);
			}

			return true;
		}

		return false;
	}
}
