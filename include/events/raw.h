#ifndef EVENTS_RAW_IN_H_
#define EVENTS_RAW_IN_H_

#include <memory>
#include <string>

#include "core/server.h"
#include "events/event.h"
#include "messages/message.h"

namespace pingpong {
	class raw_in_event: public server_event {
		public:
			std::string raw_in;

			raw_in_event(const server_ptr &serv_, const std::string &raw_in_): server_event(serv_), raw_in(raw_in_) {}
	};

	class raw_out_event: public server_event {
		public:
			std::string raw_out;

			raw_out_event(const server_ptr &serv_, const std::string &raw_out_):
				server_event(serv_), raw_out(raw_out_) {}
	};
}

#endif