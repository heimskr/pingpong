#include <string>

#include "pingpong/commands/User.h"
#include "pingpong/core/Server.h"

namespace PingPong {
	UserCommand::operator std::string() const {
		return "USER " + username + " 0 * :" + realname;
	}

	bool UserCommand::send() {
		if (Command::send()) {
			if (server->status == Server::Stage::SetUser)
				server->status = Server::Stage::SetNick;
			return true;
		}

		return false;
	}
}
