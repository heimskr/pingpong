#ifndef PINGPONG_EVENTS_LOG_H_
#define PINGPONG_EVENTS_LOG_H_

#include "pingpong/core/Channel.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched when a debug message should be logged.
	 */
	class log_event: public event {
		public:
			using event::event;
	};
}

#endif
