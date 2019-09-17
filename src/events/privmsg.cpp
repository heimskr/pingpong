#include "events/privmsg.h"

namespace pingpong {
	std::string privmsg_event::get_destination() const {
		if (!whom && !chan)
			throw std::runtime_error("Both possible destinations are null in privmsg_event");
		else if (whom && chan)
			throw std::runtime_error("Both possible destinations are non-null in privmsg_event");
		
		return whom? whom->name : chan->name;
	}

	bool privmsg_event::is_user() const {
		return whom? true : false;
	}

	bool privmsg_event::is_channel() const {
		return chan? true : false;
	}
}
