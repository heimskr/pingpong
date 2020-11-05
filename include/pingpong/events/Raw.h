#ifndef PINGPONG_EVENTS_RAW_H_
#define PINGPONG_EVENTS_RAW_H_

#include <memory>
#include <string>

#include "pingpong/core/Server.h"
#include "pingpong/events/Event.h"
#include "pingpong/messages/Message.h"

namespace PingPong {
	struct RawInEvent: public ServerEvent {
		std::string rawIn;

		RawInEvent(Server *server_, const std::string &raw_in):
			ServerEvent(server_), rawIn(raw_in) {}
	};

	struct RawOutEvent: public ServerEvent {
		std::string rawOut;

		RawOutEvent(Server *server_, const std::string &raw_out):
			ServerEvent(server_), rawOut(raw_out) {}
	};
}

#endif
