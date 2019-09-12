#include <functional>
#include <map>
#include <memory>
#include <string>

#include "events/event.h"

namespace pingpong {
	std::multimap<std::string, listener_fn> events::listeners;

	user_event::user_event(user_ptr who_, channel_ptr chan_, const std::string &content_):
	channel_event(chan_, content_), who(who_) {
		assert(who_);
		if (chan_ && who_->serv != chan_->serv)
			throw std::invalid_argument("User and channel are associated with different servers");
	}
}
