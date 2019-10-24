#ifndef PINGPONG_EVENTS_VERSION_REQUESTED_H_
#define PINGPONG_EVENTS_VERSION_REQUESTED_H_

#include "pingpong/core/local.h"
#include "pingpong/events/event.h"

namespace pingpong {
	struct version_requested_event: public local_event {
		std::shared_ptr<user> requester;

		template <typename T>
		version_requested_event(const std::shared_ptr<user> &requester_, const T &where_,
		const std::string &content_ = ""):
			local_event(requester_->serv, where_, content_), requester(requester_) {}
	};
}

#endif
