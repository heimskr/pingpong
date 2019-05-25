#include <iostream>
#include <string>

#include "core/debug.h"
#include "core/defs.h"
#include "core/irc.h"
#include "core/channel.h"
#include "core/server.h"
#include "core/user.h"
#include "lib/ansi.h"

namespace pingpong {
	void debug::print_all(const irc &obj) {
		for (server_ptr serv: obj.servers) {
			if (!serv) {
				ansi::out << ansi::red << "null" << ansi::endl;
				continue;
			}

			ansi::out << ansi::underline << ansi::yeen << serv->hostname << ansi::endl;
			for (auto [chanstr, chan]: serv->channels) {
				ansi::out << "  " << ansi::magenta << ansi::bold << chanstr << ansi::endl;
				for (auto [username, uptr]: chan->users)
					ansi::out << "    " << static_cast<char>(chan->hats[uptr]) << ansi::cyan << username << ansi::endl;
			}
		}
	}
}
