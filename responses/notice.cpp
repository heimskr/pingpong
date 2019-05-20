#include <string>

#include "notice.h"

namespace pingpong {
	std::string notice_response::get_command() const {
		return "NOTICE";
	}

	notice_response::operator std::string() const {
		return "-" + source + "- " + message;
	}
}