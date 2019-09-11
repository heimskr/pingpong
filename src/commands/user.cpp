#include <string>

#include "commands/user.h"
#include "core/server.h"

namespace pingpong {
	user_command::operator std::string() const {
		return "USER " + username + " 0 * :" + realname;
	}

	void user_command::send() {
		command::send();
		if (serv->status == server::stage::setuser)
			serv->status = server::stage::setnick;
	}
}
