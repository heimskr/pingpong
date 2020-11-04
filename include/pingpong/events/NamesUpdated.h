#ifndef PINGPONG_EVENTS_NAMESUPDATED_H_
#define PINGPONG_EVENTS_NAMESUPDATED_H_

#include "pingpong/core/Channel.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched when the collection of users in a channel is updated.
	 */
	class NamesUpdatedEvent: public ChannelEvent {
		public:
			using ChannelEvent::ChannelEvent;
	};
}

#endif
