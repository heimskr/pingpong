#include <iostream>
#include <stdexcept>
#include <string>

#include "core/mask.h"

namespace pingpong {
	mask::mask(std::string combined) {
		size_t bang, at, length = combined.size();

		for (bang = 0; combined[bang] != '!' && bang < length; ++bang);
		if (bang == length) throw std::invalid_argument("Couldn't find username separator in mask");

		for (at = bang + 1; combined[at] != '@' && at < length; ++at);
		if (at == length)   throw std::invalid_argument("Couldn't find hostname separator in mask");

		nick = combined.substr(0, bang);
		user = combined.substr(bang + 1, at - bang - 1);
		host = combined.substr(at + 1);
	}
}
