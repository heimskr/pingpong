#ifndef PINGPONG_EVENTS_MOTD_H_
#define PINGPONG_EVENTS_MOTD_H_

#include "pingpong/events/Event.h"

namespace PingPong {
	struct MotdEvent: public ServerEvent {
		MotdEvent(Server *server_, const std::string &content_): ServerEvent(server_, content_) {}
	};
}

#endif
