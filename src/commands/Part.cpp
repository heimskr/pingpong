#include "pingpong/commands/Part.h"

namespace PingPong {
	PartCommand::operator std::string() const {
		if (reason.empty())
			return "PART " + channels;
		return "PART " + channels + " :" + reason;
	}
}
