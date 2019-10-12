#include "pingpong/events/notice.h"
#include "pingpong/messages/notice.h"

namespace pingpong {
	notice_message::operator std::string() const {
		if (line.source.is_server())
			return "[" + line.source.nick + "] " + content;

		const std::string name = who? who->name : "?";
		return "[" + (is_user()? name : where) + "] -" + name + "- " + content;
	}

	bool notice_message::operator()(server *serv) {
		events::dispatch<notice_event>(serv, who, where, content);
		return true;
	}
}
