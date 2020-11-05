#include <string>

#include "pingpong/commands/Pong.h"
#include "pingpong/events/ServerStatus.h"
#include "pingpong/messages/Ping.h"

namespace PingPong {
	PingMessage::operator std::string() const {
		return "PING " + content;
	}

	bool PingMessage::operator()(Server *server) {
		PongCommand(server, content).send();
		return true;
	}
}