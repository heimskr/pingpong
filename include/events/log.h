#ifndef PINGPONG_EVENTS_LOG_H_
#define PINGPONG_EVENTS_LOG_H_

#include "core/channel.h"
#include "events/event.h"

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
