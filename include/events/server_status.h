#ifndef PINGPONG_EVENTS_LOG_H_
#define PINGPONG_EVENTS_LOG_H_

#include "core/channel.h"
#include "core/server.h"

#include "events/event.h"

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
