#include <string>

#include "messages/join.h"

namespace pingpong {
	join_message::operator std::string() const {
		return line.source.nick + " joined " + text;
	}
}