#include <string>

#include "notice.h"

namespace pingpong {
	std::string pingpong::notice_response::get_command() const {
		return "NOTICE";
	}

	std::string pingpong::notice_response::to_string() const {
		return "-" + source + "- " + message;
	}
}