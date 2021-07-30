#include "pingpong/events/ServerStatus.h"
#include "pingpong/messages/Numeric.h"

namespace PingPong {
	bool NumericMessage::handleWelcome(Server *server) {
		if (server->status == Server::Stage::SetNick || server->status == Server::Stage::CapNeg) {
			server->status = Server::Stage::Ready;
			Events::dispatch<ServerStatusEvent>(server);
		}

		return true;
	}
}
