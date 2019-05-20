#include <stdexcept>
#include <string>

#include "all.h"

namespace pingpong {
	sourced_message::sourced_message(pingpong::line line_, std::string combined): message(line_) {
		size_t i, length = combined.size();
		for (i = 0; combined[i] != ' ' && i < length; ++i)
			source += combined[i];

		for (; combined[i] == ' '; ++i);

		if (i == length || combined[i] != ':')
			throw std::runtime_error("Couldn't parse sourced_message");

		content = combined.substr(i + 1);
	}
}
