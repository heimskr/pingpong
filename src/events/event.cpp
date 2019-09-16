#include <cassert>
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

	channel_event::channel_event(channel_ptr chan_, server_ptr serv_, const std::string &content_):
	server_event(serv_, content_), chan(chan_) {
		if (chan)
			chan->serv = serv;
	}

	channel_event::channel_event(channel_ptr chan_, const std::string &content_):
	server_event(chan_? chan_->serv : nullptr, content_), chan(chan_) {
		if (chan && !chan->has_server())
			throw std::invalid_argument("Channel is not associated with a server");
	}
}
