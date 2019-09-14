#include "events/quit.h"
#include "messages/quit.h"

namespace pingpong {
	quit_message::operator std::string() const {
		return line.source.nick + " quit [" + content + "]";
	}

	bool quit_message::operator()(server_ptr serv) {
		events::dispatch<quit_event>(who, serv, content);
		return true;
	}
}
