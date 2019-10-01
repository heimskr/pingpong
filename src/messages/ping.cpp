#include <string>

#include "pingpong/commands/pong.h"
#include "pingpong/events/server_status.h"
#include "pingpong/messages/ping.h"

namespace pingpong {
	ping_message::operator std::string() const {
		return "PING " + content;
	}

	bool ping_message::operator()(server *serv) {
		pong_command(serv, content).send();
		return true;
	}
}