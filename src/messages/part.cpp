#include <string>

#include "messages/line.h"
#include "messages/part.h"

namespace pingpong {
	part_message::part_message(pingpong::line line_): message(line_) {
		std::string &params = line_.parameters;
		size_t separator = params.find(" :");

		if (separator == std::string::npos) {
			chan = params;
		} else {
			chan = params.substr(0, separator);
			reason = params.substr(separator + 2);
		}
	}

	part_message::operator std::string() const {
		std::string start = line.source.nick + " left " + chan;
		if (reason.empty()) {
			return start;
		}

		return start + " [" + reason + "]";
	}
}
