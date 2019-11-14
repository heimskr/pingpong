#ifndef PINGPONG_EVENTS_MOTD_H_
#define PINGPONG_EVENTS_MOTD_H_

#include "pingpong/events/event.h"

namespace pingpong {
	struct motd_event: public server_event {
		std::string line;
		motd_event(server *serv_, const std::string &line_): server_event(serv_), line(line_) {}
	};
}

#endif
