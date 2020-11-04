#include <iostream>
#include <string>

#include "pingpong/core/Debug.h"
#include "pingpong/core/Defs.h"
#include "pingpong/core/IRC.h"
#include "pingpong/core/Channel.h"
#include "pingpong/core/Server.h"
#include "pingpong/core/User.h"

#include "lib/formicine/ansi.h"

namespace PingPong {
	void Debug::printAll(const IRC &obj) {
		for (const Server *server: obj.serverOrder) {
			if (!server) {
				DBG(ansi::red("null") << ansi::endl);
				continue;
			}

			DBG(ansi::style::underline << ansi::yeen(server->id));
			for (std::shared_ptr<Channel> chan: server->channels) {
				DBG("    " << ansi::magenta(ansi::bold(chan->name)));
				for (std::shared_ptr<User> user: chan->users)
					DBG("        " << chan->hats[user] << ansi::cyan(user->name));
			}
		}
	}
}
