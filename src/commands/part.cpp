#include "commands/part.h"

namespace pingpong {
	part_command::operator std::string() const {
		if (reason.empty())
			return "PART " + chans;
		return "PART " + chans + " :" + reason;
	}
}
