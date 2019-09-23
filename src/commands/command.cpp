#include <string>

#include "commands/command.h"
#include "events/command.h"
	
namespace pingpong {
	std::function<bool(command &)> command::before_send = [](command &) { return true; };

	void command::send() {
		if (before_send(*this)) {
			events::dispatch<command_event>(serv, this);
			serv->quote(std::string(*this));
		}
	}
}
