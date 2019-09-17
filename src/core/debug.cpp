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
		for (server *serv: obj.servers) {
			if (!serv) {
				DBG(ansi::red("null") << ansi::endl);
				continue;
			}

			ansi::out << ansi::style::underline << ansi::yeen(serv->hostname) << ansi::endl;
			for (std::shared_ptr<channel> chan: serv->channels) {
				DBG("    " << ansi::magenta(ansi::bold(chan->name)));
				for (std::shared_ptr<user> user: chan->users)
					DBG("        " << static_cast<char>(chan->hats[user]) << ansi::cyan(user->name));
			}
		}
	}
}
