#ifndef PINGPONG_EVENTS_USER_APPEARED_H_
#define PINGPONG_EVENTS_USER_APPEARED_H_

#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Occurs whenever a user is noted to exist. If someone quits, you can listen for this event to do something the
	 * next time we see them do something, such as being seen in a channel or messaging you.
	 */
	class user_appeared_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
