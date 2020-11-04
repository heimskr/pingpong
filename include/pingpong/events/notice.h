#ifndef PINGPONG_EVENTS_NOTICE_H_
#define PINGPONG_EVENTS_NOTICE_H_

#include "pingpong/events/Event.h"

namespace PingPong {
	struct NoticeEvent: public LocalEvent {
		std::shared_ptr<User> speaker;

		/** Whether to prevent the notice from being shown in the status window. */
		bool hidden;

		template <typename T>
		NoticeEvent(const std::shared_ptr<User> &speaker_, const T &where_, const std::string &message_,
		bool hidden_ = false):
			LocalEvent(speaker_->server, where_, message_), speaker(speaker_), hidden(hidden_) {}

		template <typename T>
		NoticeEvent(Server *server_, const std::shared_ptr<User> &speaker_, const T &where_,
		const std::string &message_, bool hidden_ = false):
			LocalEvent(server_, where_, message_), speaker(speaker_), hidden(hidden_) {}
	};
}

#endif
