#include <algorithm>

#include "pingpong/messages/Targeted.h"
#include "pingpong/core/Server.h"

namespace PingPong {
	TargetedMessage::TargetedMessage(const PingPong::Line &line_): Message(line_) {
		// Example parameters for a kick: "#channel whom :reason"
		
		const std::string &raw = line_.parameters;
		if (raw.empty())
			throw std::runtime_error("Line parameters are empty in TargetedMessage");

		// The smallest possible valid message is "# w :".
		if (raw.length() < 5)
			throw std::runtime_error("Line parameters are too short for TargetedMessage");
		
		if (std::count(raw.begin(), raw.end(), ' ') < 2)
			throw std::runtime_error("Not enough parameters for TargetedMessage");

		const auto end = raw.end();
		const auto first = std::find(raw.begin(), raw.end(), ' ');
		const auto second = std::find(first + 1, raw.end(), ' ');
		if (first == end || second == end)
			throw std::runtime_error("Invalid TargetedMessage: not enough spaces");

		if (*(second + 1) != ':')
			throw std::runtime_error("Invalid TargetedMessage: second space not followed by colon");

		who     = line_.server->getUser(line_.source.nick, true, true);
		whom    = line_.server->getUser(std::string(first + 1, second), true, true);
		channel = line_.server->getChannel(std::string(raw.begin(), first), true);
		content = std::string(second + 2, end);
	}
}
