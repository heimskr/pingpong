#include <iostream>
#include <stdexcept>
#include <string>

#include "core/mask.h"

namespace pingpong {
	mask::mask(std::string combined) {
		size_t bang, at, length = combined.size();

		bang = combined.find('!');
		if (bang == std::string::npos) {
			if (combined.find('@') != std::string::npos)
				throw std::invalid_argument("Couldn't find hostname separator in mask");
			nick = combined;
			return;
		}

		at = combined.find('@');
		if (at == length) throw std::invalid_argument("Couldn't find hostname separator in mask");
		if (at < bang)
			throw std::invalid_argument("Hostname separator came before username separator in mask");

		nick = combined.substr(0, bang);
		user = combined.substr(bang + 1, at - bang - 1);
		host = combined.substr(at + 1);
	}

	bool mask::is_full() const {
		return !nick.empty() && !user.empty() && !host.empty();
	}

	bool mask::is_empty() const {
		return nick.empty() && user.empty() && host.empty();
	}

	mask::operator std::string() const {
		if (is_full())
			return nick + "!" + user + "@" + host;
		return nick;
	}
}
