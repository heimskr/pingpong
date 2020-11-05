#include <string>

#include "pingpong/commands/Quit.h"
#include "pingpong/events/Quit.h"

namespace PingPong {
	QuitCommand::operator std::string() const {
		return "QUIT :" + reason;
	}

	bool QuitCommand::send() {
		if (Command::send()) {
			Events::dispatch<QuitEvent>(server->getSelf());
			return true;
		}

		return false;
	}
}
