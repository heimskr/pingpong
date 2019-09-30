#include <string>

#include "pingpong/events/part.h"
#include "pingpong/events/names_updated.h"

#include "pingpong/messages/part.h"

namespace pingpong {
	part_message::operator std::string() const {
		return who->name + " left " + where + (content.empty()? "" : " [" + content + "]");
	}

	bool part_message::operator()(server *serv) {
		if (where.empty() || where.front() != '#')
			throw std::runtime_error("Invalid channel for part_message");
		
		std::shared_ptr<channel> chan = serv->get_channel(where, true);

		if (who->is_self()) {
			serv->remove_channel(where);
		} else {
			chan->remove_user(who);
			events::dispatch<names_updated_event>(chan);
		}

		events::dispatch<part_event>(who, chan, content);
		return true;
	}
}
