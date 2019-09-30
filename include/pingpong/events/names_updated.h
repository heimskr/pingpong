#ifndef PINGPONG_EVENTS_NAMES_UPDATED_H_
#define PINGPONG_EVENTS_NAMES_UPDATED_H_

#include "pingpong/core/channel.h"
#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Dispatched when the collection of users in a channel is updated.
	 */
	class names_updated_event: public channel_event {
		public:
			using channel_event::channel_event;
	};
}

#endif
