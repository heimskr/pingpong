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
	class privmsg_event: public local_event {
		public:
			std::shared_ptr<user> speaker;

			template <typename T>
			privmsg_event(const std::shared_ptr<user> &speaker_, const T &where_, const std::string &message_):
				local_event(speaker_->serv, where_, message_), speaker(speaker_) {}
	};
}

#endif
