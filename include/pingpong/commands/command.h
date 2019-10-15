#ifndef PINGPONG_COMMANDS_COMMAND_H_
#define PINGPONG_COMMANDS_COMMAND_H_

#include <functional>

#include "pingpong/core/util.h"

namespace pingpong {
	class server;
	class channel;

	class command {
		public:
			// Preprocesses a command and determines whether it's okay to send.
			static std::function<bool(command &)> before_send;

			server *serv;
			long sent_time = util::timestamp();

			command(server *serv_): serv(serv_) {}
			virtual operator std::string() const = 0;

			/** Tries to send the command. Returns true if the command was sent. Implementations should run before_send
			 *  to check whether it's okay to send the command with the base class's implementation. */
			virtual bool send();

			virtual bool is_silent() const { return false; }
	};
}

#endif
