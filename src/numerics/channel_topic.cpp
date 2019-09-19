#include "events/topic_updated.h"

#include "messages/numeric.h"

namespace pingpong {
	/** Parses a channel topic message like "#channel :topic" */
	bool numeric_message::handle_channel_topic(server *serv) {
		std::string name, chan_name, topic_text;
		std::tie(name, chan_name, topic_text) = parse_ssc(line);

		std::shared_ptr<channel> chan = serv->get_channel(chan_name, true);
		if (chan->topic.text != topic_text) {
			topicset old_topic = chan->topic;
			chan->topic.text = topic_text;
			events::dispatch<topic_updated_event>(chan, old_topic, chan->topic);
		}

		return true;
	}
}
