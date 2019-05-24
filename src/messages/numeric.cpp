#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "core/debug.h"
#include "lib/ansi.h"
#include "messages/numeric.h"

namespace pingpong {
	numeric_message::numeric_message(pingpong::line line_): message(line_) {
		if (!is_numeric(line_.command.c_str(), number))
			throw std::invalid_argument("Expected a numeric command");
	}

	numeric_message::operator std::string() const {
		return "[" + std::to_string(number) + "] " + line.original;
	}

	void numeric_message::operator()(server_ptr) const {
		if (number == 353) {
			irc::dbg << ansi::yellow;
			irc::dbg << "[" << line.parameters << "]\n" << ansi::reset;

			try {
				numeric_message::parse353(line.parameters);
			} catch (const std::invalid_argument &err) {
				YIKES("Couldn't parse 353 message");
			}

			irc::dbg << "\n" << ansi::reset;
		}
	}

	numeric_message::names numeric_message::parse353(const std::string &params) {
		size_t sep = params.find(" = ");
		channel::visibility vis = channel::visibility::pub;

		if (sep == std::string::npos) {
			sep = params.find(" * ");
			if (sep == std::string::npos) {
				sep = params.find(" @ ");
				vis = channel::visibility::secret;
			} else {
				vis = channel::visibility::priv;
			}
		}

		size_t colon = params.find(':');
		if (sep == std::string::npos || colon == std::string::npos || colon < sep) {
			YIKES((sep == std::string::npos) << ", " << (colon == std::string::npos) << ", " << (colon < sep));
			throw std::invalid_argument("Invalid 353 message");
		}

		std::string chanstr, userstr;
		try {
			chanstr = params.substr(sep + 3, colon - sep - 4);
			userstr = params.substr(colon + 1);
			while (userstr.back() == ' ')
				userstr.pop_back();
		} catch (const std::out_of_range &err) {
			throw std::invalid_argument("Invalid 353 message");
		}

		std::vector<std::pair<user::hat, std::string>> userlist;
		while (userstr.length() > 0) {
			size_t space = userstr.find(' ');
			std::string to_add;
			if (space == std::string::npos) {
				to_add = userstr;
				userstr.clear();
			} else {
				to_add = userstr.substr(0, space);
				for (; userstr[space] == ' '; ++space);
				userstr.erase(0, space);
			}

			user::hat userhat = user::get_hat(to_add);
			if (userhat != user::hat::none)
				to_add.erase(0, 1);
			userlist.push_back({userhat, to_add});
		}

		return {chanstr, vis, userlist};
	}
	
	bool numeric_message::is_numeric(const char *str) {
		if (!std::isdigit(str[0])) return false;
		char *ptr;
		strtol(str, &ptr, 10);
		return !*ptr;
	}

	bool numeric_message::is_numeric(const char *str, long &l) {
		if (!std::isdigit(str[0])) return false;
		char *ptr;
		long result = strtol(str, &ptr, 10);
		if (*ptr)
			return false;
		l = result;
		return true;
	}
}
