#ifndef PINGPONG_EVENTS_QUIT_H_
#define PINGPONG_EVENTS_QUIT_H_

#include "core/channel.h"
#include "events/event.h"

namespace pingpong {
	class quit_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
