#include <iostream>
#include <stdexcept>
#include <string>

#include "pingpong/core/mask.h"
#include "pingpong/core/parse_error.h"

namespace pingpong {
	mask::mask(const std::string &combined) {
		const size_t length = combined.size();
		const size_t bang = combined.find('!');

		if (bang == std::string::npos) {
			if (combined.find('@') != std::string::npos)
				throw parse_error("Couldn't find hostname separator in mask");
			nick = combined;
			return;
		}

		const size_t at = combined.find('@');

		if (at == length)
			throw parse_error("Couldn't find hostname separator in mask");

		if (at < bang)
			throw parse_error("Hostname separator came before username separator in mask");

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

	bool mask::is_server() const {
		return !nick.empty() && user.empty() && host.empty();
	}

	mask::operator std::string() const {
		if (is_full())
			return nick + "!" + user + "@" + host;
		return nick;
	}
}
