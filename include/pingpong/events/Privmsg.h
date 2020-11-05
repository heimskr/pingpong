#ifndef PINGPONG_EVENTS_PRIVMSG_H_
#define PINGPONG_EVENTS_PRIVMSG_H_

#include "pingpong/core/Channel.h"
#include "pingpong/core/Local.h"
#include "pingpong/core/User.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Represents a message to a user or a channel.
	 */
	struct PrivmsgEvent: public LocalEvent {
		std::shared_ptr<User> speaker;

		template <typename T>
		PrivmsgEvent(const std::shared_ptr<User> &speaker_, const T &where_, const std::string &message_):
			LocalEvent(speaker_->server, where_, message_), speaker(speaker_) {}
	};
}

#endif
