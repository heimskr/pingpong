#include <iostream>
#include <string>

#include "messages/join.h"
#include "core/server.h"

namespace pingpong {
	join_message::operator std::string() const {
		return line.source.nick + " joined " + content;
	}

	void join_message::operator()(server_ptr serv) const {
		if (!serv->has_channel(content))
			*serv += content;
	}
}