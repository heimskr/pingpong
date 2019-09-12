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
				ansi::out << ansi::color::red << "null" << ansi::endl;
				continue;
			}

			ansi::out << ansi::style::underline << ansi::color::yeen << serv->hostname << ansi::endl;
			for (auto [chanstr, chan]: serv->channels) {
				ansi::out << "  " << ansi::color::magenta << ansi::style::bold << chanstr << ansi::endl;
				for (auto [username, uptr]: chan->users)
					ansi::out << "    " << static_cast<char>(chan->hats[uptr]) << ansi::color::cyan << username << ansi::endl;
			}
		}
	}
}
