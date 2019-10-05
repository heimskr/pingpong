#include <string>

#include "pingpong/commands/quit.h"
#include "pingpong/events/quit.h"

namespace pingpong {
	quit_command::operator std::string() const {
		return "QUIT :" + reason;
	}

	bool quit_command::send() {
		if (command::send()) {
			events::dispatch<quit_event>(serv->get_self());
			return true;
		}

		return false;
	}
}
