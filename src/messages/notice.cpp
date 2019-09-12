#include <string>

#include "messages/notice.h"

namespace pingpong {
	notice_message::operator std::string() const {
		return "[" + chan->name + "] -" + who->name + "- " + content;
	}
}