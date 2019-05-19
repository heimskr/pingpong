#ifndef COMMANDS_PRIVMSG_H_
#define COMMANDS_PRIVMSG_H_

#include <string>

#include "command.h"

namespace pingpong {
	class privmsg_command: public command {
		public:
			channel destination;
			std::string message;

			privmsg_command(channel, std::string);
			privmsg_command(serv_ptr, std::string, std::string);

			virtual std::string to_string() const;
	};
}

#endif
