#include "pingpong/events/server_status.h"
#include "pingpong/messages/numeric.h"

namespace pingpong {
	bool numeric_message::handle_welcome(server *serv) {
		if (serv->status == server::stage::setnick) {
			serv->status = server::stage::ready;
			events::dispatch<server_status_event>(serv);
		}

		return true;
	}
}
