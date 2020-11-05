#include "pingpong/events/Topic.h"
#include "pingpong/events/TopicUpdated.h"
#include "pingpong/messages/Topic.h"

namespace PingPong {
	TopicMessage::operator std::string() const {
		return who->name + " changed the topic of " + where + " to \"" + content + "\"";
	}

	bool TopicMessage::operator()(Server *server) {
		if (where.empty() || where.front() != '#')
			throw std::runtime_error("Invalid channel for TopicMessage");

		std::shared_ptr<Channel> chan = server->getChannel(where, true);
		TopicSet old_topic = chan->topic;
		chan->topic = content;
		Events::dispatch<TopicUpdatedEvent>(chan, old_topic, chan->topic);
		Events::dispatch<TopicEvent>(who, chan, content);

		return true;
	}
}
