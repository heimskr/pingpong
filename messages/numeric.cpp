#include <string>

#include "messages/numeric.h"

namespace pingpong {
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

	std::string numeric_message::get_command() const {
		return "_NUMERIC";
	}

	numeric_message::operator std::string() const {
		return "[" + std::to_string(number) + "] " + line.original;
	}
}
