#include <string>

#include "events/privmsg.h"
#include "messages/privmsg.h"

namespace pingpong {
	privmsg_message::operator std::string() const {
		return "[" + (chan? chan->name : "?") + "] <" + (who? who->name : "?") + "> " + content;
	}

	bool privmsg_message::operator()(server_ptr) {
		events::dispatch<privmsg_event>(who, chan, content);
		return true;
	}
}