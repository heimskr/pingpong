#ifndef PINGPONG_EVENTS_KICK_H_
#define PINGPONG_EVENTS_KICK_H_

#include "pingpong/core/channel.h"
#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Dispatched when a user kicks another user from a channel.
	 */
	class kick_event: public targeted_event {
		public:
			using targeted_event::targeted_event;
	};
}

#endif
