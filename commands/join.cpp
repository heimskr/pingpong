#include <stdexcept>
#include <string>

#include "join.h"

namespace pingpong {
	join_command::join_command(channel chan): command(chan.serv), pairs({{chan, ""}}) {}
	join_command::join_command(server_ptr ptr, std::string chan): command(ptr), pairs({{channel(chan, ptr), ""}}) {}
	join_command::join_command(server_ptr ptr, std::vector<join_pair> pairs_): command(ptr), pairs(pairs_) {}

	join_command::join_command(server_ptr ptr, std::vector<std::string> chans): command(ptr) {
		for (const std::string &chan: chans)
			pairs.push_back({chan, ""});
	}

	join_command::join_command(server_ptr ptr, std::vector<channel> chans): command(ptr) {
		for (const channel &chan: chans) {
			if (chan.serv != ptr) {
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
				if (!iter->second.empty()) {
					include_keys = true;
				}
			}

			if (include_keys) {
				return "JOIN " + chans + " " + keys;
			}

			return "JOIN " + chans;
		}

		throw std::runtime_error("Invalid join command");
	}
}
