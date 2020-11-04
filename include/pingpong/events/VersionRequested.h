#ifndef PINGPONG_EVENTS_VERSION_REQUESTED_H_
#define PINGPONG_EVENTS_VERSION_REQUESTED_H_

#include "pingpong/core/Local.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	struct VersionRequestedEvent: public LocalEvent {
		std::shared_ptr<User> requester;

		template <typename T>
		VersionRequestedEvent(const std::shared_ptr<User> &requester_, const T &where_,
		const std::string &content_ = ""):
			LocalEvent(requester_->server, where_, content_), requester(requester_) {}
	};
}

#endif
