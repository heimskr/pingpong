#include "pingpong/events/Quit.h"
#include "pingpong/messages/Quit.h"

namespace PingPong {
	QuitMessage::operator std::string() const {
		return line.source.nick + " quit [" + content + "]";
	}

	bool QuitMessage::operator()(Server *server) {
		if (server != who->server)
			throw std::runtime_error("Invalid server for user");

		if (who == server->getSelf())
			server->reap();
		else
			server->removeUser(who);

		Events::dispatch<QuitEvent>(who, content);
		return true;
	}
}
