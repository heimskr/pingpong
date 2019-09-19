#ifndef PINGPONG_EVENTS_TOPIC_UPDATED_H_
#define PINGPONG_EVENTS_TOPIC_UPDATED_H_

#include "core/topicset.h"
#include "events/event.h"

namespace pingpong {
	struct topic_updated_event: public channel_event {
		topicset old_topic, new_topic;

		topic_updated_event(const std::shared_ptr<channel> &chan_, const topicset &old_, const topicset &new_):
			channel_event(chan_, new_.text), old_topic(old_), new_topic(new_) {}
	};
}

#endif
