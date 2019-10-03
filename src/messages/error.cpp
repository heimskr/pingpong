#include "pingpong/messages/error.h"

namespace pingpong {
	error_message::operator std::string() const {
		return ansi::wrap("-"_d + "!" + "- "_d, ansi::color::red) + content;
	}

	bool error_message::operator()(server *serv) {
		// When sent to a client, the ERROR message indicates a disconnection. It should remove the server.
		serv->reap();
		return true;
	}
}