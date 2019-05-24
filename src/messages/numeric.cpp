#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>

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

	std::pair<std::string, std::unordered_set<std::string>> numeric_message::parse353(const std::string &params) {
		size_t equals = params.find(" = ");
		size_t colon = params.find(':');
		if (equals == std::string::npos || colon == std::string::npos || colon < equals)
			throw std::invalid_argument("Invalid 353 message");

		try {
			std::string chanstr = params.substr(equals + 3, colon - equals - 4);
			std::string userlist = params.substr(colon + 1);
			while (userlist.back() == ' ')
				userlist.pop_back();

			irc::dbg << ansi::magenta << "[" << params.substr(0, equals) << "]";
			irc::dbg << ansi::cyan << " [" << chanstr << "]";
			irc::dbg << ansi::yeen << " [" << userlist << "]";

			return {"", {}};
		} catch (const std::out_of_range &err) {
			YIKES("Encountered out_of_range while parsing 353 message");
			throw std::invalid_argument("Invalid 353 message");
		}
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
