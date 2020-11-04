#include <stdexcept>
#include <string>

#include "pingpong/commands/Join.h"
#include "pingpong/commands/Mode.h"
#include "pingpong/core/Channel.h"

namespace PingPong {
	JoinCommand::JoinCommand(const Channel &channel): JoinCommand(channel.server, {JoinPair(channel, "")}) {}

	JoinCommand::JoinCommand(Server *server_, const std::vector<std::string> &channels): Command(server_) {
		for (const std::string &channel: channels)
			pairs.push_back({channel, ""});
	}

	JoinCommand::JoinCommand(Server *server_, const std::vector<Channel> &channels): Command(server_) {
		for (const Channel &channel: channels) {
			if (channel.server != server_)
				throw std::runtime_error("Can't join channels on multiple servers simultaneously");
			pairs.push_back({channel, ""});
		}
	}

	JoinCommand::operator std::string() const {
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

	bool JoinCommand::send() {
		if (Command::send()) {
			for (const JoinPair &pair: pairs)
				ModeCommand(pair.first, server).send();
			return true;
		}

		return false;
	}
}
