#ifndef COMMANDS_USER_H_
#define COMMANDS_USER_H_

#include <memory>

#include "command.h"

namespace pingpong {
	class user_command: public command {
		public:
			user_command(serv_ptr serv_): command(serv_) { }
	};
}

#endif
