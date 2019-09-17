#include <functional>
#include <map>
#include <memory>
#include <string>

#include "events/event.h"

namespace pingpong {
	std::multimap<std::string, listener_fn> events::listeners;

	channel_event::channel_event(std::shared_ptr<channel> chan_, server *serv_,
	const std::string &content_): server_event(serv_, content_), chan(chan_) {
		if (chan)
			chan->serv = serv;
	}

	channel_event::channel_event(std::shared_ptr<channel> chan_, const std::string &content_):
	server_event(chan_? chan_->serv : nullptr, content_), chan(chan_) {
		if (chan && !chan->has_server())
			throw std::invalid_argument("Channel is not associated with a server in channel_event");
	}

	user_event::user_event(std::shared_ptr<user> who_, std::shared_ptr<channel> chan_, const std::string &content_):
	channel_event(chan_, content_), who(who_) {
		if (chan_ && who_->serv != chan_->serv)
			throw std::invalid_argument("User and channel are associated with different servers in user_event");
	}

	targeted_event::targeted_event(std::shared_ptr<user> who_, std::shared_ptr<user> whom_,
	std::shared_ptr<channel> chan_, const std::string &content_): user_event(who_, chan_, content_), whom(whom_) {
		if (who->serv != whom->serv)
			throw std::invalid_argument("Users are associated with different servers in targeted_event");
	}
}
