#include <string>

#include "commands/part.h"

namespace pingpong {
	part_command::operator std::string() const {
		if (reason.empty())
			return "PART " + chan->name;
		return "PART " + chan->name + " :" + reason;
	}
}
