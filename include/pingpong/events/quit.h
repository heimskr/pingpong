#ifndef PINGPONG_EVENTS_QUIT_H_
#define PINGPONG_EVENTS_QUIT_H_

#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Occurs whenever a user quits (QUIT).
	 */
	class QuitEvent: public UserEvent {
		public:
			using UserEvent::UserEvent;
	};
}

#endif
