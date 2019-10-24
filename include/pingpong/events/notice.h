#ifndef PINGPONG_EVENTS_NOTICE_H_
#define PINGPONG_EVENTS_NOTICE_H_

#include "pingpong/events/event.h"

namespace pingpong {
	struct notice_event: public local_event {
		std::shared_ptr<user> speaker;

		/** Whether to prevent the notice from being shown in the status window. */
		bool hidden;

		template <typename T>
		notice_event(const std::shared_ptr<user> &speaker_, const T &where_, const std::string &message_,
		bool hidden_ = false):
			local_event(speaker_->serv, where_, message_), speaker(speaker_), hidden(hidden_) {}

		template <typename T>
		notice_event(server *serv_, const std::shared_ptr<user> &speaker_, const T &where_,
		const std::string &message_, bool hidden_ = false):
			local_event(serv_, where_, message_), speaker(speaker_), hidden(hidden_) {}
	};
}

#endif
