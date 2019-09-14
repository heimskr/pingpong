#include <string>

#include "messages/part.h"
#include "events/part.h"

namespace pingpong {
	part_message::operator std::string() const {
		std::string start = line.source.nick + " left " + chan->name;
		if (content.empty()) {
			return start;
		}

		return start + " [" + content + "]";
	}

	bool part_message::operator()(server_ptr serv) {
		*serv -= chan->name;
		events::dispatch<part_event>(who, chan, content);
		return true;
	}
}
