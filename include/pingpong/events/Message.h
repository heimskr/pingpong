#ifndef PINGPONG_EVENTS_MESSAGE_H_
#define PINGPONG_EVENTS_MESSAGE_H_

#include <memory>
#include <string>

#include "pingpong/core/Server.h"
#include "pingpong/events/Event.h"
#include "pingpong/messages/Message.h"

namespace PingPong {
	class MessageEvent: public ServerEvent {
		public:
			std::shared_ptr<Message> msg;

			MessageEvent(Server *server_, std::shared_ptr<Message> msg_): ServerEvent(server_), msg(msg_) {}
	};
}

#endif
