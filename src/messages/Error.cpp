#include "pingpong/messages/Error.h"
#include "pingpong/core/Server.h"

namespace PingPong {
	ErrorMessage::operator std::string() const {
		return ansi::wrap("-"_d + "!" + "- "_d, ansi::color::red) + content;
	}

	bool ErrorMessage::operator()(Server *server) {
		// When sent to a client, the ERROR message indicates a disconnection. It should remove the server.
		server->reap();
		return true;
	}
}