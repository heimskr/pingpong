#ifndef PINGPONG_EVENTS_TOPIC_H_
#define PINGPONG_EVENTS_TOPIC_H_

#include "events/event.h"

namespace pingpong {
	/**
	 * Dispatched whenever a user changes a channel's topic. This event, not topic_updated_event, should trigger a
	 * "Someone changed the topic"-type message.
	 */
	struct topic_event: public channel_event {
		using channel_event::channel_event;
	};
}

#endif
