#include "pingpong/events/privmsg.h"
#include "pingpong/messages/privmsg.h"

namespace pingpong {
	privmsg_message::operator std::string() const {
		return "[" + where + "] <" + who->name + "> " + content;
	}

	bool privmsg_message::operator()(server *) {
		events::dispatch<privmsg_event>(who, where, content);
		return true;
	}
}