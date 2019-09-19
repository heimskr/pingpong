#ifndef PINGPONG_EVENTS_QUIT_H_
#define PINGPONG_EVENTS_QUIT_H_

#include "events/event.h"

namespace pingpong {
	/**
	 * Occurs whenever a user quits (QUIT).
	 */
	class quit_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
