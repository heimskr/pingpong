#ifndef PINGPONG_EVENTS_HATS_UPDATED_H_
#define PINGPONG_EVENTS_HATS_UPDATED_H_

#include "pingpong/core/Defs.h"
#include "pingpong/core/Hats.h"

#include "pingpong/events/Event.h"

namespace PingPong {
	struct HatsUpdatedEvent: public UserEvent {
		HatSet oldHats, newHats;
		
		HatsUpdatedEvent(const std::shared_ptr<User> &who_, const std::shared_ptr<Channel> &channel_,
		const HatSet &old_hats, const HatSet &new_hats):
			UserEvent(who_, channel_, std::string(new_hats)), oldHats(old_hats), newHats(new_hats) {}
	};
}

#endif
