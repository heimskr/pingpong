#include <string>

#include "messages/notice.h"

namespace pingpong {
	std::string notice_message::get_command() const {
		return "NOTICE";
	}

	notice_message::operator std::string() const {
		return "-" + source + "- " + content;
	}
}