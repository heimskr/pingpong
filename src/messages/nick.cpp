#include "pingpong/events/Nick.h"
#include "pingpong/messages/Nick.h"

namespace PingPong {
	NickMessage::operator std::string() const {
		return line.source.nick + " is now known as " + content;
	}

	bool NickMessage::operator()(Server *server) {
		if (server != who->server)
			throw std::runtime_error("Invalid server for user");

		server->renameUser(line.source.nick, content);
		Events::dispatch<NickEvent>(who, line.source.nick);
		return true;
	}
}
