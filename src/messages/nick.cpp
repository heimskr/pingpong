#include "messages/nick.h"

namespace pingpong {
	nick_message::operator std::string() const {
		return line.source.nick + " is now known as " + content;
	}

	void nick_message::operator()(server_ptr serv) const {
		serv->rename_user(line.source.nick, content);
	}
}
