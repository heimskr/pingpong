#include <iostream>
#include <string>

#include "core/debug.h"
#include "core/defs.h"
#include "core/irc.h"
#include "core/channel.h"
#include "core/server.h"

namespace pingpong {
	void debug::print_all(const irc &obj) {
		for (server_ptr serv: obj.servers) {
			std::cout << "S ";
			if (!serv) {
				std::cout << "null\n";
				continue;
			}

			std::cout << serv->hostname << "\n";
			for (auto [chanstr, chan]: serv->channels) {
				std::cout << "  C " << chanstr << "\n";
				for (auto [username, user]: chan->users)
					std::cout << "    U " << username << "\n";
			}
		}
	}
}
