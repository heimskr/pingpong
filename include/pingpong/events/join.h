#ifndef PINGPONG_EVENTS_JOIN_H_
#define PINGPONG_EVENTS_JOIN_H_

#include "pingpong/core/Channel.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	class JoinEvent: public UserEvent {
		public:
			using UserEvent::UserEvent;
	};
}

#endif
