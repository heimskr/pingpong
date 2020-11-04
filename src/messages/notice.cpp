#include "pingpong/events/Notice.h"
#include "pingpong/messages/Notice.h"

namespace PingPong {
	NoticeMessage::operator std::string() const {
		if (line.source.isServer())
			return "[" + line.source.nick + "] " + content;

		const std::string name = who? who->name : "?";
		return "[" + (isUser()? name : where) + "] -" + name + "- " + content;
	}

	bool NoticeMessage::operator()(Server *serv) {
		Events::dispatch<NoticeEvent>(serv, who, where, content);
		return true;
	}
}
