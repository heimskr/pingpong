#include "events/nick.h"
#include "messages/nick.h"

namespace pingpong {
	nick_message::operator std::string() const {
		return line.source.nick + " is now known as " + content;
	}

	bool nick_message::operator()(server *serv) {
		serv->rename_user(line.source.nick, content);
		events::dispatch<nick_event>(who, get_server(), line.source.nick);
		return true;
	}
}
