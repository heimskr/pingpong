#include <string>

#include "messages/line.h"
#include "core/mask.h"

#include "lib/ansi.h"

namespace pingpong {
	line::line(server *serv_, const std::string &in): original(in), serv(serv_), source(mask("", "", "")) {
		size_t index = 0;

		std::string in_ = in;

		if (in_[0] == '@') {
			index = in_.find(' ');
			tags = in_.substr(1, index - 1);
			in_.erase(0, index + 1);
		}

		if (in_[index] == ':') {
			index = in_.find(' ');
			source = mask(in_.substr(1, index - 1));
			in_.erase(0, index + 1);
		}

		index = in_.find(' ');
		command    = in_.substr(0, index);
		parameters = in_.substr(index + 1);
	}
	
	line::operator std::string() const {
		return "T[" + tags + "], S[" + std::string(source) + "], C[" + command + "], P[" + parameters + "]";
	}

	std::ostream & operator<<(std::ostream &os, const line &line_) {
		return os << std::string(line_);
	}
}
