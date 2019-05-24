#include <string>
#include "commands/command.h"
	
namespace pingpong {
	void command::send(bool silent) {
		serv->quote(std::string(*this), silent);
	}

	void command::send() {
		send(false);
	}
}
