#include <stdexcept>
#include <string>

#include "channel.h"

namespace pingpong {
	channel::channel(std::string name_): name(name_) {
		if (name_ == "") throw std::invalid_argument("Invalid channel name");
	}

	bool channel::is_user() {
		return name.at(0) != '#';
	}
}
