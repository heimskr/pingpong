#include "messages/notice.h"

namespace pingpong {
	notice_message::operator std::string() const {
		return "[" + (chan? chan->name : "?") + "] -" + (who? who->name : "?") + "- " + content;
	}
}