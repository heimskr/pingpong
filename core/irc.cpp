#include <string>

#include "core/irc.h"
#include "messages/all.h"

namespace pingpong {
	void irc::init_messages() {
		message::add_ctor<join_message>();
		message::add_ctor<notice_message>();
		message::add_ctor<numeric_message>();
		message::add_ctor<part_message>();
		message::add_ctor<ping_message>();
		message::add_ctor<privmsg_message>();
	}
}
