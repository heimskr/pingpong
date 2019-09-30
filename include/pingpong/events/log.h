#ifndef PINGPONG_EVENTS_LOG_H_
#define PINGPONG_EVENTS_LOG_H_

#include "pingpong/core/channel.h"
#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Dispatched when a debug message should be logged.
	 */
	class log_event: public event {
		public:
			using event::event;
	};
}

#endif
