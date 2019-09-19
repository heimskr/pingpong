#include "events/topic.h"
#include "events/topic_updated.h"
#include "messages/topic.h"

namespace pingpong {
	topic_message::operator std::string() const {
		return who->name + " changed the topic of " + where + " to \"" + content + "\"";
	}

	bool topic_message::operator()(server *serv) {
		if (where.empty() || where.front() != '#')
			throw std::runtime_error("Invalid channel for topic_message");

		std::shared_ptr<channel> chan = serv->get_channel(where, true);
		topicset old_topic = chan->topic;
		chan->topic = content;
		events::dispatch<topic_updated_event>(chan, old_topic, chan->topic);
		events::dispatch<topic_event>(who, chan, content);

		return true;
	}
}
