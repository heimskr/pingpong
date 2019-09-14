#include <string>

#include "commands/pong.h"
#include "messages/ping.h"

namespace pingpong {
	ping_message::operator std::string() const {
		return "PING " + content;
	}

	bool ping_message::operator()(server_ptr serv) {
		pong_command(serv, content).send();
		return true;
	}
}