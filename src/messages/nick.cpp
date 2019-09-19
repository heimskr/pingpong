#include "events/nick.h"
#include "messages/nick.h"

namespace pingpong {
	nick_message::operator std::string() const {
		return line.source.nick + " is now known as " + content;
	}

	bool nick_message::operator()(server *serv) {
		if (serv != who->serv)
			throw std::runtime_error("Invalid server for user");

		serv->rename_user(line.source.nick, content);
		events::dispatch<nick_event>(who, line.source.nick);
		return true;
	}
}
