#include <iostream>
#include <string>

#include "messages/join.h"
#include "core/server.h"

namespace pingpong {
	join_message::operator std::string() const {
		return line.source.nick + " joined " + text;
	}

	void join_message::operator()(server_ptr serv) const {
		*serv += text;
	}
}