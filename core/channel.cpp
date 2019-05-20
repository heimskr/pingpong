#include <stdexcept>
#include <string>

#include "core/defs.h"
#include "core/channel.h"
#include "core/server.h"

namespace pingpong {
	channel::channel(std::string name_, server_ptr serv_): name(name_), serv(serv_) {
		if (name_ == "") throw std::invalid_argument("Invalid channel name");
	}

	channel::channel(std::string name_): channel(name_, nullptr) {}

	bool channel::is_user() {
		return name.at(0) != '#';
	}
}
