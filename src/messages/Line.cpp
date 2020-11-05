#include <string>

#include "pingpong/messages/Line.h"
#include "pingpong/core/Mask.h"

#include "lib/formicine/ansi.h"

namespace PingPong {
	Line::Line(Server *server_, const std::string &in): original(in), server(server_), source(Mask("", "", "")) {
		size_t index = 0;

		std::string in_ = in;

		if (in_[0] == '@') {
			index = in_.find(' ');
			tags = in_.substr(1, index - 1);
			in_.erase(0, index + 1);
		}

		if (in_[index] == ':') {
			index = in_.find(' ');
			source = Mask(in_.substr(1, index - 1));
			in_.erase(0, index + 1);
		}

		index      = in_.find(' ');
		command    = in_.substr(0, index);
		parameters = in_.substr(index + 1);
	}
	
	Line::operator std::string() const {
		return "T[" + tags + "], S[" + std::string(source) + "], C[" + command + "], P[" + parameters + "]";
	}

	Line::operator bool() const {
		return server && !original.empty();
	}

	std::ostream & operator<<(std::ostream &os, const Line &line_) {
		return os << std::string(line_);
	}
}
