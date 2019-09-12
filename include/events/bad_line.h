#ifndef EVENTS_BAD_LINE_H_
#define EVENTS_BAD_LINE_H_

#include <string>

#include "core/server.h"
#include "events/event.h"
#include "messages/message.h"

namespace pingpong {
	class bad_line_event: public server_event {
		public:
			std::string bad_line;

			bad_line_event(const server_ptr &serv_, const std::string &bad_line_):
				server_event(serv_), bad_line(bad_line_) {}
	};
}

#endif
