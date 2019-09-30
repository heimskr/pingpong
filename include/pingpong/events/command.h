#ifndef PINGPONG_EVENTS_COMMAND_H_
#define PINGPONG_EVENTS_COMMAND_H_

#include <string>

#include "pingpong/core/server.h"
#include "pingpong/events/event.h"
#include "pingpong/messages/message.h"
#include "pingpong/commands/command.h"

namespace pingpong {
	/**
	 * Occurs after a command is sent to a server.
	 */
	class command_event: public server_event {
		public:
			command *cmd;
			command_event(server *serv_, command *cmd_): server_event(serv_), cmd(cmd_) {}
	};
}

#endif
