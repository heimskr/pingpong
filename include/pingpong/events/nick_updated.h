#ifndef PINGPONG_EVENTS_NICK_UPDATED_H_
#define PINGPONG_EVENTS_NICK_UPDATED_H_

#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Dispatched whenever a user's nickname is updated by any means other than a NICK message.
	 * For example, this should be used when the case of a user's name is corrected.
	 */
	struct nick_updated_event: public event {
		std::shared_ptr<pingpong::user> who;
		std::string old_nick, new_nick;

		nick_updated_event(const std::shared_ptr<pingpong::user> &who_, const std::string &old_nick_,
		const std::string &new_nick_):
			who(who_), old_nick(old_nick_), new_nick(new_nick_) {}
	};
}

#endif
