#include <string>

#include "commands/command.h"
#include "events/command.h"
	
namespace pingpong {
	void command::send() {
		events::dispatch<command_event>(serv, this);
		serv->quote(std::string(*this));
	}
}
