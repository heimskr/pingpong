#include <string>

#include "events/part.h"
#include "events/names_updated.h"

#include "messages/part.h"

namespace pingpong {
	part_message::operator std::string() const {
		std::string start = line.source.nick + " left " + chan->name;
		if (content.empty()) {
			return start;
		}

		return start + " [" + content + "]";
	}

	bool part_message::operator()(server *serv) {
		if (who->is_self()) {
			*serv -= chan->name;
		} else {
			*chan -= who;
			events::dispatch<names_updated_event>(chan);
		}

		events::dispatch<part_event>(who, chan, content);
		return true;
	}
}
