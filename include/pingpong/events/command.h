#ifndef PINGPONG_EVENTS_COMMAND_H_
#define PINGPONG_EVENTS_COMMAND_H_

#include <string>

#include "pingpong/core/Server.h"
#include "pingpong/events/Event.h"
#include "pingpong/messages/Message.h"
#include "pingpong/commands/Command.h"

namespace PingPong {
	/**
	 * Occurs after a command is sent to a server.
	 */
	struct CommandEvent: public ServerEvent {
		Command *command;
		CommandEvent(Server *server_, Command *command_):
			ServerEvent(server_), command(command_) {}
	};
}

#endif
