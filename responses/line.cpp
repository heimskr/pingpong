#include "line.h"

namespace pingpong {
	line::line(std::string in) {
		size_t length = in.size(), index = 0, old_index;

		if (in[index] == '@') {
			for (; in[index] != ' ' && index < length; ++index);
			tags = in.substr(1, index++ - 1);
		}

		if (in[index] == ':') {
			for (old_index = index; in[index] != ' ' && index < length; ++index);
			source = in.substr(old_index + 1, index++ - old_index - 1);
		}

		for (old_index = index; in[index] != ' ' && index < length; ++index);
		command    = in.substr(old_index, index - old_index);
		parameters = in.substr(index + 1);
	}
}
