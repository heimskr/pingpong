#ifndef PINGPONG_COMMANDS_COMMAND_H_
#define PINGPONG_COMMANDS_COMMAND_H_

#include <functional>

#include "pingpong/core/Util.h"

namespace PingPong {
	class Server;
	class Channel;

	class Command {
		public:
			// Preprocesses a command and determines whether it's okay to send.
			static std::function<bool(Command &)> beforeSend;

			Server *server;
			long sentTime = Util::timestamp();
			std::string label;


			Command & setLabel(const std::string &);

			/** Randomizes the command's label. */
			Command & setLabel();

			virtual operator std::string() const = 0;

			/** Tries to send the command. Returns true if the command was sent. Implementations should run beforeSend
			 *  to check whether it's okay to send the command with the base class's implementation. */
			virtual bool send();

			virtual bool isSilent() const { return false; }

		protected:
			Command(Server *server_): server(server_) {
				setLabel();
			}
	};
}

#endif
