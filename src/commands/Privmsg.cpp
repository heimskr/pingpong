#include "pingpong/commands/Privmsg.h"

#include "pingpong/events/Error.h"
#include "pingpong/events/Privmsg.h"

namespace PingPong {
	PrivmsgCommand::PrivmsgCommand(Server *server_, const std::string &where_, const std::string &message_):
	Command(server_), Local(where_), message(message_) {
		if (where.empty())
			throw std::runtime_error("Destination is empty in PrivmsgCommand");
	}

	PrivmsgCommand::operator std::string() const {
		return "PRIVMSG " + where + " :" + message;
	}

	bool PrivmsgCommand::send() {
		if (Command::send() && !server->featureEnabled(PingPong::Features::Type::EchoMessage)) {
			if (isChannel()) {
				std::shared_ptr<Channel> channel = getChannel(server);
				if (channel)
					Events::dispatch<PrivmsgEvent>(server->getSelf(), channel, message, Line());
				else
					Events::dispatch<ErrorEvent>("Can't send message: channel is null", false);
			} else {
				Events::dispatch<PrivmsgEvent>(server->getSelf(), getUser(server, false), message, Line());
			}

			return true;
		}

		return false;
	}
}
