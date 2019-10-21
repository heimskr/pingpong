#ifndef PINGPONG_EVENTS_NICK_IN_USE_H_
#define PINGPONG_EVENTS_NICK_IN_USE_H_

#include "pingpong/events/event.h"

namespace pingpong {
	struct nick_in_use_event: public server_event {
		std::string nick;

		nick_in_use_event(const std::string &nick_, pingpong::server *serv_):
			server_event(serv_), nick(nick_) {}
	};
}

#endif
