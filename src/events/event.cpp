#include <functional>
#include <map>
#include <memory>
#include <string>

#include "pingpong/events/Event.h"

namespace PingPong {
	std::multimap<std::string, std::pair<std::string, Listener_f>> Events::listeners;
	size_t Events::listenersAdded = 0;

	ChannelEvent::ChannelEvent(const std::shared_ptr<Channel> &channel_, Server *server_, const std::string &content_):
	ServerEvent(server_, content_), channel(channel_) {
		if (channel && !channel->hasServer())
			throw std::invalid_argument("Channel is not associated with a server in ChannelEvent");
	}

	UserEvent::UserEvent(const std::shared_ptr<User> &who_, const std::shared_ptr<Channel> &channel_,
	const std::string &content_): ChannelEvent(channel_, who_->server, content_), who(who_) {
		if (channel_ && who_->server != channel_->server)
			throw std::invalid_argument("User and channel are associated with different servers in UserEvent");
	}

	TargetedEvent::TargetedEvent(const std::shared_ptr<User> &who_, const std::shared_ptr<User> &whom_,
	const std::shared_ptr<Channel> &channel_, const std::string &content_):
	UserEvent(who_, channel_, content_), whom(whom_) {
		if (who->server != whom->server)
			throw std::invalid_argument("Users are associated with different servers in TargetedEvent");
	}
}
