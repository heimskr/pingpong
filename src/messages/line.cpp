#include <string>

#include "messages/line.h"
#include "core/mask.h"

namespace pingpong {
	line::line(server_ptr serv_, const std::string &in): serv(serv_), original(in), source(mask("", "", "")) {
		size_t index = 0;

		if (in[0] == '@') {
			index = in.find(' ');
			tags = in.substr(1, index - 1);
			in.erase(0, index + 1);
		}

		if (in[index] == ':') {
			index = in.find(' ');
			source = mask(in.substr(1, index - 1));
			in.erase(0, index + 1);
		}

		index = in.find(' ');
		command    = in.substr(0, index);
		parameters = in.substr(index + 1);
	}
	
	line::operator std::string() const {
		return "T[" + tags + "], S[" + std::string(source) + "], C[" + command + "], P[" + parameters + "]";
	}
}
