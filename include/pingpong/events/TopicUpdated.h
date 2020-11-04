#ifndef PINGPONG_EVENTS_TOPIC_UPDATED_H_
#define PINGPONG_EVENTS_TOPIC_UPDATED_H_

#include "pingpong/core/TopicSet.h"
#include "pingpong/events/Event.h"

namespace PingPong {
	/**
	 * Dispatched whenever a channel's topic is changed. This isn't just via TOPIC commands; numeric 332 specifies the
	 * topic for a channel on join. This event shouldn't be used to display a "Someone changed the topic"-type message.
	 */
	struct TopicUpdatedEvent: public ChannelEvent {
		TopicSet old_topic, new_topic;

		TopicUpdatedEvent(const std::shared_ptr<Channel> &chan_, const TopicSet &old_, const TopicSet &new_):
			ChannelEvent(chan_, new_.text), old_topic(old_), new_topic(new_) {}
	};
}

#endif
