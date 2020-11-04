#ifndef PINGPONG_EVENTS_LOG_H_
#define PINGPONG_EVENTS_LOG_H_

#include "pingpong/core/Channel.h"
#include "pingpong/core/Server.h"

#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched when a server's status changes, such as when it's connected or removed.
	 */
	class ServerStatusEvent: public ServerEvent {
		public:
			// Equal to the server's status at the time of dispatch if the previous status isn't known.
			const Server::Stage priorStatus;

			ServerStatusEvent(Server *server_, Server::Stage prior_status):
				ServerEvent(server_), priorStatus(prior_status) {}

			ServerStatusEvent(Server *server_):
				ServerStatusEvent(server_, server_->getStatus()) {}
	};
}

#endif
