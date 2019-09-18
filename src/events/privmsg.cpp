#include "events/privmsg.h"

namespace pingpong {
	privmsg_event::privmsg_event(std::shared_ptr<user> speaker_, std::string where_, const std::string &message_):
	server_event(speaker_->serv, message_), local(where_), speaker(speaker_) {
		if (where.empty())
			throw std::runtime_error("Location cannot be empty in privmsg_event");
	}
}
