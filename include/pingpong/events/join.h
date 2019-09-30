#ifndef PINGPONG_EVENTS_JOIN_H_
#define PINGPONG_EVENTS_JOIN_H_

#include "pingpong/core/channel.h"
#include "pingpong/events/event.h"

namespace pingpong {
	class join_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
