#ifndef PINGPONG_EVENTS_NICK_IN_USE_H_
#define PINGPONG_EVENTS_NICK_IN_USE_H_

#include "pingpong/events/Event.h"

namespace PingPong {
	struct NickInUseEvent: public ServerEvent {
		std::string nick;

		NickInUseEvent(const std::string &nick_, Server *server_):
			ServerEvent(server_), nick(nick_) {}
	};
}

#endif
