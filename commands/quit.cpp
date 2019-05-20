#include <string>

#include "commands/quit.h"

namespace pingpong {
	quit_command::operator std::string() const {
		return "QUIT :" + reason;
	}
}
