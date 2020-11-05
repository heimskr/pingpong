#include <iostream>
#include <stdexcept>
#include <string>

#include "pingpong/core/Mask.h"
#include "pingpong/core/ParseError.h"

namespace PingPong {
	Mask::Mask(const std::string &combined) {
		const size_t length = combined.size();
		const size_t bang = combined.find('!');

		if (bang == std::string::npos) {
			if (combined.find('@') != std::string::npos)
				throw ParseError(combined, "Couldn't find hostname separator in mask");
			nick = combined;
			return;
		}

		const size_t at = combined.find('@');

		if (at == length)
			throw ParseError(combined, "Couldn't find hostname separator in mask");

		if (at < bang)
			throw ParseError(combined, "Hostname separator came before username separator in mask");

		nick = combined.substr(0, bang);
		user = combined.substr(bang + 1, at - bang - 1);
		host = combined.substr(at + 1);
	}

	bool Mask::isFull() const {
		return !nick.empty() && !user.empty() && !host.empty();
	}

	bool Mask::isEmpty() const {
		return nick.empty() && user.empty() && host.empty();
	}

	bool Mask::isServer() const {
		return !nick.empty() && user.empty() && host.empty();
	}

	Mask::operator std::string() const {
		if (isFull())
			return nick + "!" + user + "@" + host;
		return nick;
	}
}
