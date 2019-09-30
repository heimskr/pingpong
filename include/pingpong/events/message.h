#ifndef PINGPONG_EVENTS_MESSAGE_H_
#define PINGPONG_EVENTS_MESSAGE_H_

#include <memory>
#include <string>

#include "pingpong/core/server.h"
#include "pingpong/events/event.h"
#include "pingpong/messages/message.h"

namespace pingpong {
	class message_event: public server_event {
		public:
			std::shared_ptr<message> msg;

			message_event(server *serv_, std::shared_ptr<message> msg_): server_event(serv_), msg(msg_) {}
	};
}

#endif
