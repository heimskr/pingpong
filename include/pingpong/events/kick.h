#ifndef PINGPONG_EVENTS_KICK_H_
#define PINGPONG_EVENTS_KICK_H_

#include "pingpong/core/Channel.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched when a user kicks another user from a channel.
	 */
	class KickEvent: public TargetedEvent {
		public:
			using TargetedEvent::TargetedEvent;
	};
}

#endif
