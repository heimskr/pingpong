#include <string>

#include "messages/join.h"

namespace pingpong {
	std::string join_message::get_command() const {
		return "JOIN";
	}

	join_message::operator std::string() const {
		return line.source.nick + " joined " + text;
	}
}