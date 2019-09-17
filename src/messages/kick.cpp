#include "events/kick.h"
#include "events/names_updated.h"
#include "messages/kick.h"

namespace pingpong {
	kick_message::operator std::string() const {
		return who->name + " kicked " + whom->name + " from " + chan->name + " [" + content + "]";
	}

	bool kick_message::operator()(server *) {
		*chan -= whom;
		events::dispatch<names_updated_event>(chan);
		events::dispatch<kick_event>(who, whom, chan, content);
		return true;
	}
}
