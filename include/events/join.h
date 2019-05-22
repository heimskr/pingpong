#ifndef EVENTS_JOIN_H_
#define EVENTS_JOIN_H_

#include "core/channel.h"
#include "events/events.h"

namespace pingpong {
	class join_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
