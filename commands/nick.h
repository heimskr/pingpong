#ifndef COMMANDS_NICK_H_
#define COMMANDS_NICK_H_

#include "command.h"

namespace pingpong {
	using std::string;

	class nick_command: public command {
		public:
			string nickname;

			nick_command(serv_ptr serv_, string nickname_): command(serv_), nickname(nickname_) {}
			virtual string to_string() const;
	};
}

#endif
