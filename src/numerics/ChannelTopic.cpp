#include "pingpong/events/TopicUpdated.h"

#include "pingpong/messages/Numeric.h"

namespace PingPong {
	/** Parses a channel topic message like "#channel :topic" */
	bool NumericMessage::handleChannelTopic(Server *server) {
		std::string chan_name, topic_text;
		std::tie(std::ignore, chan_name, topic_text) = parseSSC(line);

		std::shared_ptr<Channel> channel = server->getChannel(chan_name, true);
		if (channel->topic.text != topic_text) {
			TopicSet old_topic = channel->topic;
			channel->topic.text = topic_text;
			Events::dispatch<TopicUpdatedEvent>(channel, old_topic, channel->topic);
		}

		return true;
	}
}
