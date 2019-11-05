#include "pingpong/events/privmsg.h"
#include "pingpong/events/version_requested.h"
#include "pingpong/messages/privmsg.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	privmsg_message::operator std::string() const {
		return "[" + where + "] <" + who->name + "> " + content;
	}

	bool privmsg_message::operator()(server *serv) {
		if (formicine::util::upper(content) == "\x01VERSION\x01") {
			events::dispatch<version_requested_event>(who, where, content);
		} else {
			events::dispatch<privmsg_event>(who, where, content);
		}

		return true;
	}
}