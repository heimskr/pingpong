#ifndef PINGPONG_EVENTS_BAD_LINE_H_
#define PINGPONG_EVENTS_BAD_LINE_H_

#include <string>

#include "pingpong/core/server.h"
#include "pingpong/events/event.h"
#include "pingpong/messages/message.h"

namespace pingpong {
	class bad_line_event: public server_event {
		public:
			std::string bad_line;

			bad_line_event(server *serv_, const std::string &bad_line_): server_event(serv_), bad_line(bad_line_) {}
	};
}

#endif
