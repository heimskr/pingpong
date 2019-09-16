#include <iostream>
#include <string>

#include "core/server.h"
#include "events/join.h"
#include "events/names_updated.h"
#include "messages/join.h"

namespace pingpong {
	join_message::join_message(const pingpong::line &line_): sourced_message(line_) {
		chan = line_.serv->get_channel(content, true);
		content = "";
	}

	join_message::operator std::string() const {
		return who->name + " joined " + chan->name;
	}

	bool join_message::operator()(server_ptr serv) {
		if (!serv->has_channel(chan->name))
			*serv += chan->name;

		if (!chan->has_user(who)) {
			*chan += who;
			events::dispatch<names_updated_event>(chan);
		}
		
		events::dispatch<join_event>(who, chan);
		return true;
	}
}