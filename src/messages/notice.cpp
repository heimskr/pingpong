#include <string>

#include "messages/notice.h"

namespace pingpong {
	notice_message::operator std::string() const {
		return "[" + source + "] -" + line.source.nick + "- " + content;
	}
}