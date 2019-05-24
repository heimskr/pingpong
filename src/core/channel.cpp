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

	bool channel::operator==(const std::string &str) const {
		return name == str;
	}

	bool channel::operator==(const channel &chan) const {
		return name == chan.name && serv == chan.serv;
	}

	bool channel::operator<(const channel &chan) const {
		return name < chan.name;
	}

	bool channel::has_server() const {
		return serv != nullptr;
	}

	std::ostream & operator<<(std::ostream &os, const channel &chan) {
		os << std::string(chan);
		return os;
	}
}

