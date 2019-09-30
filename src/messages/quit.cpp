#include "pingpong/events/quit.h"
#include "pingpong/messages/quit.h"

namespace pingpong {
	quit_message::operator std::string() const {
		return line.source.nick + " quit [" + content + "]";
	}

	bool quit_message::operator()(server *serv) {
		if (serv != who->serv)
			throw std::runtime_error("Invalid server for user");

		serv->remove_user(who);
		events::dispatch<quit_event>(who, content);
		return true;
	}
}
