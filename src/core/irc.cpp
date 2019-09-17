#include <string>

#include "core/irc.h"
#include "core/server.h"

#include "events/server_status.h"

#include "messages/error.h"
#include "messages/join.h"
#include "messages/kick.h"
#include "messages/nick.h"
#include "messages/notice.h"
#include "messages/numeric.h"
#include "messages/quit.h"
#include "messages/part.h"
#include "messages/ping.h"
#include "messages/privmsg.h"

#include "lib/ansi.h"

namespace pingpong {
	ansi::ansistream irc::dbg;

	irc::~irc() {
		for (server *serv: servers)
			delete serv;
	}

	void irc::init() {
		init_messages();
	}

	void irc::init_messages() {
		message::add_ctor<error_message>();
		message::add_ctor<join_message>();
		message::add_ctor<kick_message>();
		message::add_ctor<nick_message>();
		message::add_ctor<notice_message>();
		message::add_ctor<numeric_message>();
		message::add_ctor<quit_message>();
		message::add_ctor<part_message>();
		message::add_ctor<ping_message>();
		message::add_ctor<privmsg_message>();
	}

	irc & irc::operator+=(server_ptr serv) {
		if (servers.count(serv) == 0) {
			servers.insert(serv);
			if (!active_server)
				active_server = serv;
			events::dispatch<server_status_event>(serv);
		}

		return *this;
	}
}
