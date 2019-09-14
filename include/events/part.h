#ifndef PINGPONG_EVENTS_PART_H_
#define PINGPONG_EVENTS_PART_H_

#include "core/channel.h"
#include "events/event.h"

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
