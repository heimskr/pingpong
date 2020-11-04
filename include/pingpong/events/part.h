#ifndef PINGPONG_EVENTS_PART_H_
#define PINGPONG_EVENTS_PART_H_

#include "pingpong/core/Channel.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched when a user leaves a channel.
	 */
	class PartEvent: public UserEvent {
		public:
			using UserEvent::UserEvent;
	};
}

#endif
