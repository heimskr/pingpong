#include <stdexcept>
#include <string>

#include "channel.h"
#include "join.h"

namespace pingpong {

	join_command::join_command(channel chan): command(chan.serv), pairs({{chan, ""}}) {}
	join_command::join_command(serv_ptr ptr, string chan): command(ptr), pairs({{channel(chan, ptr), ""}}) {}
	join_command::join_command(serv_ptr ptr, std::vector<join_pair> pairs_): command(ptr), pairs(pairs_) {}

	join_command::join_command(serv_ptr ptr, std::vector<string> chans): command(ptr) {
		for (const string &chan: chans)
			pairs.push_back({chan, ""});
	}

	join_command::join_command(serv_ptr ptr, std::vector<channel> chans): command(ptr) {
		for (const channel &chan: chans) {
			if (chan.serv != ptr) {
				throw std::runtime_error("Can't join channels on multiple servers simultaneously");
			}

			pairs.push_back({chan, ""});
		}
	}

	std::string join_command::to_string() const {
		if (pairs.size() > 0) {
			string chans = pairs.at(0).first;
			string keys = pairs.at(0).second;

			bool include_keys = false;

			for (auto iter = pairs.begin() + 1; iter != pairs.end(); ++iter) {
				chans += "," + string(iter->first);
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
