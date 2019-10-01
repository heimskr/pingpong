#include <iostream>
#include <string>

#include "pingpong/core/debug.h"
#include "pingpong/core/defs.h"
#include "pingpong/core/irc.h"
#include "pingpong/core/channel.h"
#include "pingpong/core/server.h"
#include "pingpong/core/user.h"

#include "lib/formicine/ansi.h"

namespace pingpong {
	void debug::print_all(const irc &obj) {
		for (const server *serv: obj.server_order) {
			if (serv == nullptr) {
				DBG(ansi::red("null") << ansi::endl);
				continue;
			}

			DBG(ansi::style::underline << ansi::yeen(serv->id));
			for (std::shared_ptr<channel> chan: serv->channels) {
				DBG("    " << ansi::magenta(ansi::bold(chan->name)));
				for (std::shared_ptr<user> user: chan->users)
					DBG("        " << static_cast<char>(chan->hats[user]) << ansi::cyan(user->name));
			}
		}
	}
}
