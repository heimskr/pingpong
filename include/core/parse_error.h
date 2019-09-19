#ifndef PINGPONG_CORE_PARSE_ERROR_H_
#define PINGPONG_CORE_PARSE_ERROR_H_

#include <stdexcept>

namespace pingpong {
	struct parse_error: public std::exception {
		const std::string message;

		parse_error(const std::string &message_): message(message_) {}

		const char * what() const noexcept {
			return message.c_str();
		}
	};
}

#endif
