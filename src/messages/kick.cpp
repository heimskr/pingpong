#include "pingpong/events/kick.h"
#include "pingpong/events/names_updated.h"
#include "pingpong/messages/kick.h"

namespace pingpong {
	kick_message::operator std::string() const {
		return who->name + " kicked " + whom->name + " from " + chan->name + " [" + content + "]";
	}

	bool kick_message::operator()(server *) {
		chan->remove_user(whom);
		events::dispatch<names_updated_event>(chan);
		events::dispatch<kick_event>(who, whom, chan, content);
		return true;
	}
}
