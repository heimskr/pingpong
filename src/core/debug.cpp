#include <iostream>
#include <string>

#include "core/debug.h"
#include "core/ppdefs.h"
#include "core/irc.h"
#include "core/channel.h"
#include "core/server.h"
#include "core/user.h"
#include "lib/ansi.h"

namespace pingpong {
	void debug::print_all(const irc &obj) {
		for (server_ptr serv: obj.servers) {
			if (!serv) {
				DBG(ansi::red("null") << ansi::endl);
				continue;
			}

			ansi::out << ansi::style::underline << ansi::yeen(serv->hostname) << ansi::endl;
			for (channel_ptr chan: serv->channels) {
				DBG("    " << ansi::magenta(ansi::bold(chan->name)));
				for (user_ptr user: chan->users)
					DBG("        " << static_cast<char>(chan->hats[user]) << ansi::cyan(user->name));
			}
		}
	}
}
