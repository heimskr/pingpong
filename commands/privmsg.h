#ifndef COMMANDS_PRIVMSG_H_
#define COMMANDS_PRIVMSG_H_

#include <string>

#include "command.h"

namespace pingpong {
	using std::string;

	class privmsg_command: public command {
		public:
			channel destination;
			string message;

			privmsg_command(channel, string);
			privmsg_command(serv_ptr, string, string);

			virtual string to_string() const;
	};
}

#endif
