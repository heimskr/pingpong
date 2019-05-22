#include <ostream>
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

	bool channel::is_user() const {
		return name[0] != '#';
	}

	std::ostream & operator<<(std::ostream &os, const channel &chan) {
		os << std::string(chan);
		return os;
	}
}

