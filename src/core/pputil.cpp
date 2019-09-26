#include <cctype>

#include "core/pputil.h"

namespace pingpong {
	std::chrono::system_clock::duration util::now() {
		return std::chrono::system_clock::now().time_since_epoch();
	}

	long util::timestamp() {
		return std::chrono::duration_cast<std::chrono::seconds>(now()).count();
	}

	long util::millistamp() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(now()).count();
	}

	long util::microstamp() {
		return std::chrono::duration_cast<std::chrono::microseconds>(now()).count();
	}

	long util::nanostamp() {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(now()).count();
	}

	bool util::is_valid_nick(const std::string &str) {
		return !str.empty() && str.find_first_not_of(nick_chars) == std::string::npos;
	}

	std::string & util::rtrim(std::string &str) {
		str.erase(std::find_if(str.rbegin(), str.rend(), [](char c) {
			return !std::isspace(c) && c != '\n' && c != '\r' && c != '\t';
		}).base(), str.end());
		return str;
	}

	std::string util::rtrim(const std::string &str) {
		std::string copy = str;
		return rtrim(copy);
	}
}
