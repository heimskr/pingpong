#include "pingpong/commands/Mode.h"

namespace PingPong {
	ModeCommand::operator std::string() const {
		return "MODE " + where + (!flags.empty()? " " + flags : "") + (!extra.empty()? " " + extra : "");
	}
}
