#ifndef PINGPONG_EVENTS_JOIN_H_
#define PINGPONG_EVENTS_JOIN_H_

#include "core/channel.h"
#include "events/event.h"

namespace pingpong {
	class join_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
