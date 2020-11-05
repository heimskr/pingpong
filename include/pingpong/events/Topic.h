#ifndef PINGPONG_EVENTS_TOPIC_H_
#define PINGPONG_EVENTS_TOPIC_H_

#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched whenever a user changes a channel's topic. This event, not TopicUpdatedEvent, should trigger a
	 * "Someone changed the topic"-type message.
	 */
	struct TopicEvent: public UserEvent {
		using UserEvent::UserEvent;
	};
}

#endif
