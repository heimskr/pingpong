#ifndef PINGPONG_EVENTS_PART_H_
#define PINGPONG_EVENTS_PART_H_

#include "pingpong/core/channel.h"
#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Dispatched when a user leaves a channel.
	 */
	class part_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
