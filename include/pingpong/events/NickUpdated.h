#ifndef PINGPONG_EVENTS_NICK_UPDATED_H_
#define PINGPONG_EVENTS_NICK_UPDATED_H_

#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched whenever a user's nickname is updated by any means other than a NICK message.
	 * For example, this should be used when the case of a user's name is corrected.
	 */
	struct NickUpdatedEvent: public Event {
		std::shared_ptr<User> who;
		std::string oldNick, newNick;

		NickUpdatedEvent(const std::shared_ptr<PingPong::User> &who_, const std::string &old_nick,
		const std::string &new_nick):
			who(who_), oldNick(old_nick), newNick(new_nick) {}
	};
}

#endif
