#include <stdexcept>
#include <string>

#include "all.h"

namespace pingpong {
	pingpong::sourced_response::sourced_response(pingpong::line line_, std::string combined): response(line_) {
		size_t i, length = combined.size();
		for (i = 0; combined[i] != ' ' && i < length; ++i)
			source += combined[i];

		for (; combined[i] == ' '; ++i);

		if (i == length)
			throw std::runtime_error("Couldn't parse sourced_response");

		message = combined.substr(i);
	}
}
