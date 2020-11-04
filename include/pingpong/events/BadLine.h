#ifndef PINGPONG_EVENTS_BAD_LINE_H_
#define PINGPONG_EVENTS_BAD_LINE_H_

#include <string>

#include "pingpong/core/Server.h"
#include "pingpong/events/Event.h"
#include "pingpong/messages/Message.h"

namespace PingPong {
	class BadLineEvent: public ServerEvent {
		public:
			std::string badLine;

			BadLineEvent(Server *server_, const std::string &bad_line): ServerEvent(server_), badLine(bad_line) {}
	};
}

#endif
