#pragma once

#include "pingpong/core/Channel.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	/** Represents a nick change. In this event, the user pointer points to the user after their nick has been updated,
	 *  while the content contains the old nickname. */
	class NickEvent: public UserEvent {
		public:
			using UserEvent::UserEvent;
	};
}
