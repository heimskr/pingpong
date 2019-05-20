#include <stdexcept>
#include <string>

#include "defs.h"
#include "channel.h"
#include "server.h"

namespace pingpong {
	channel::channel(std::string name_, server_ptr serv_): name(name_), serv(serv_) {
		if (name_ == "") throw std::invalid_argument("Invalid channel name");
	}

	channel::channel(std::string name_): channel(name_, nullptr) {}

	bool channel::is_user() {
		return name.at(0) != '#';
	}
}
