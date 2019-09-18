#ifndef PINGPONG_EVENTS_PRIVMSG_H_
#define PINGPONG_EVENTS_PRIVMSG_H_

#include "core/channel.h"
#include "core/local.h"
#include "core/user.h"
#include "events/event.h"

namespace pingpong {
	/**
	 * Represents a message to a user or a channel.
	 */
	class privmsg_event: public server_event, public local {
		public:
			enum class type {channel, user};
			std::shared_ptr<user> speaker;

			privmsg_event(std::shared_ptr<user> speaker_, std::string where_, const std::string &message_);

			privmsg_event(std::shared_ptr<user> speaker_, std::shared_ptr<user> whom, const std::string &message_):
				privmsg_event(speaker_, whom->name, message_) {}

			privmsg_event(std::shared_ptr<user> speaker_, std::shared_ptr<channel> chan, const std::string &message_):
				privmsg_event(speaker_, chan->name, message_) {}
	};
}

#endif
