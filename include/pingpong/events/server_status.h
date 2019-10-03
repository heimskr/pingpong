#ifndef PINGPONG_EVENTS_LOG_H_
#define PINGPONG_EVENTS_LOG_H_

#include "pingpong/core/channel.h"
#include "pingpong/core/server.h"

#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Dispatched when a server's status changes, such as when it's connected or removed.
	 */
	class server_status_event: public server_event {
		public:
			// Equal to the server's status at the time of dispatch if the previous status isn't known.
			const server::stage prior_status;

			server_status_event(server *serv_, server::stage prior_status_):
				server_event(serv_), prior_status(prior_status_) {}

			server_status_event(server *serv_):
				server_status_event(serv_, serv_->get_status()) {}
	};
}

#endif
