#include <string>

#include "pingpong/commands/Command.h"
#include "pingpong/events/Command.h"
	
namespace PingPong {
	std::function<bool(Command &)> Command::beforeSend = [](Command &) { return true; };

	bool Command::send() {
		if (beforeSend(*this)) {
			Events::dispatch<CommandEvent>(server, this);
			server->quote(std::string(*this));
			return true;
		}

		return false;
	}
}
