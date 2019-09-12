#include "messages/error.h"

namespace pingpong {
	error_message::operator std::string() const {
		return ansi::wrap("-"_d + "!" + "- "_d, ansi::color::red) + content;
	}
}