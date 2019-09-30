#include "pingpong/commands/mode.h"

namespace pingpong {
	mode_command::operator std::string() const {
		return "MODE " + where + (!flags.empty()? " " + flags : "") + (!extra.empty()? " " + extra : "");
	}
}
