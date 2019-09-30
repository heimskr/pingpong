#include <stdexcept>
#include <string>

#include "pingpong/commands/join.h"
#include "pingpong/commands/mode.h"

namespace pingpong {
	join_command::join_command(server *serv_, std::vector<join_pair> pairs_): command(serv_), pairs(pairs_) {}

	join_command::join_command(channel chan): command(chan.serv) {
		pairs = {{chan, ""}};
	}

	join_command::join_command(server *serv_, std::string chan): command(serv_) {
		pairs = {{chan, ""}};
	}

	join_command::join_command(server *serv_, std::vector<std::string> chans): command(serv_) {
		for (const std::string &chan: chans)
			pairs.push_back({chan, ""});
	}

	join_command::join_command(server *serv_, std::vector<channel> chans): command(serv_) {
		for (const channel &chan: chans) {
			if (chan.serv != serv_) {
				throw std::runtime_error("Can't join channels on multiple servers simultaneously");
			}

			pairs.push_back({chan, ""});
		}
	}

	join_command::operator std::string() const {
		if (pairs.size() > 0) {
			std::string chans = pairs.at(0).first;
			std::string keys = pairs.at(0).second;

			bool include_keys = false;

			for (auto iter = pairs.begin() + 1; iter != pairs.end(); ++iter) {
				chans += "," + std::string(iter->first);
				keys += "," + iter->second;
				if (!iter->second.empty())
					include_keys = true;
			}

			if (include_keys) {
				return "JOIN " + chans + " " + keys;
			}

			return "JOIN " + chans;
		}

		throw std::runtime_error("Invalid join command");
	}

	bool join_command::send() {
		if (command::send()) {
			for (const join_pair &pair: pairs)
				mode_command(pair.first, serv).send();
			return true;
		}

		return false;
	}
}
