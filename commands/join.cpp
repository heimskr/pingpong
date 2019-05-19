#include <string>

#include "channel.h"
#include "join.h"

namespace pingpong {

	join_command::join_command(channel chan): command(chan.serv), pairs({{chan, ""}}) {}
	join_command::join_command(serv_ptr ptr, string chan): command(ptr), pairs({{channel(chan, ptr), ""}}) {}
	join_command::join_command(serv_ptr ptr, std::vector<join_pair> pairs_): command(ptr), pairs(pairs_) {}

	join_command::join_command(serv_ptr ptr, std::vector<string> chans): command(ptr) {
		for (string &chan: chans)
			pairs.push_back({channel(chan, ptr), ""});
	}

	join_command::join_command(serv_ptr ptr, std::vector<channel> chans): command(ptr) {
		for (channel &chan: chans)
			pairs.push_back({chan, ""});
	}


	std::string join_command::to_string() const {
		// return "join " + destination.name + " :" + message;
		return "";
	}
}
