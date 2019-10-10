#ifndef PINGPONG_EVENTS_NOTICE_H_
#define PINGPONG_EVENTS_NOTICE_H_

#include "pingpong/events/event.h"

namespace pingpong {
	struct notice_event: public local_event {
		std::shared_ptr<user> speaker;

		template <typename T>
		notice_event(const std::shared_ptr<user> &speaker_, const T &where_, const std::string &message_):
			local_event(speaker_->serv, where_, message_), speaker(speaker_) {}

		template <typename T>
		notice_event(server *serv_, const std::shared_ptr<user> &speaker_, const T &where_,
		const std::string &message_):
			local_event(serv_, where_, message_), speaker(speaker_) {}
	};
}

#endif
