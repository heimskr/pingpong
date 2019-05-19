#ifndef COMMANDS_USER_H_
#define COMMANDS_USER_H_

#include <memory>

#include "command.h"

namespace pingpong {
	using std::string;

	class user_command: public command {
		public:
			string username;
			string realname;

			user_command(serv_ptr serv_, string username_, string realname_):
				command(serv_), username(username_), realname(realname_) { }

			virtual string to_string() const;
	};
}

#endif
