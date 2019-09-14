#include <iostream>
#include <string>

#include "core/server.h"
#include "events/join.h"
#include "messages/join.h"

namespace pingpong {
	join_message::join_message(const pingpong::line &line_): sourced_message(line_) {
		chan = line_.serv->get_channel(content, true);
		content = "";
	}

	join_message::operator std::string() const {
		std::string who_ = who->name;
		std::string chan_ = chan->name;
		return who_ + " joined " + chan_;
	}

	bool join_message::operator()(server_ptr serv) {
		if (!serv->has_channel(chan->name))
			*serv += chan->name;
		events::dispatch<join_event>(who, chan);
		return true;
	}
}