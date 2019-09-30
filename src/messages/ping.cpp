#include <string>

#include "pingpong/commands/pong.h"
#include "pingpong/events/server_status.h"
#include "pingpong/messages/ping.h"

namespace pingpong {
	ping_message::operator std::string() const {
		return "PING " + content;
	}

	bool ping_message::operator()(server *serv) {
		if (serv->status == server::stage::setnick) {
			serv->status = server::stage::ready;
			events::dispatch<server_status_event>(serv);
		}

		pong_command(serv, content).send();
		return true;
	}
}