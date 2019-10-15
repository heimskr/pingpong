#include <algorithm>

#include "pingpong/messages/targeted.h"
#include "pingpong/core/server.h"

namespace pingpong {
	targeted_message::targeted_message(const pingpong::line &line_): message(line_) {
		// Example parameters for a kick: "#channel whom :reason"
		
		const std::string &raw = line_.parameters;
		if (raw.empty())
			throw std::runtime_error("Line parameters are empty in targeted_message");

		// The smallest possible valid message is "# w :".
		if (raw.length() < 5)
			throw std::runtime_error("Line parameters are too short for targeted_message");
		
		if (std::count(raw.begin(), raw.end(), ' ') < 2)
			throw std::runtime_error("Not enough parameters for targeted_message");

		const auto end = raw.end();
		const auto first = std::find(raw.begin(), raw.end(), ' ');
		const auto second = std::find(first + 1, raw.end(), ' ');
		if (first == end || second == end)
			throw std::runtime_error("Invalid targeted_message: not enough spaces");

		if (*(second + 1) != ':')
			throw std::runtime_error("Invalid targeted_message: second space not followed by colon");

		who     = line_.serv->get_user(line_.source.nick, true);
		chan    = line_.serv->get_channel(std::string(raw.begin(), first), true);
		whom    = line_.serv->get_user(std::string(first + 1, second), true);
		content = std::string(second + 2, end);
	}
}
