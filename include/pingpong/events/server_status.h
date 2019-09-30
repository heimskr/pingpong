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
			using server_event::server_event;
	};
}

#endif
