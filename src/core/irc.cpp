#include <string>

#include "core/irc.h"
#include "core/server.h"

#include "events/server_status.h"

#include "messages/topic.h"
#include "messages/error.h"
#include "messages/join.h"
#include "messages/kick.h"
#include "messages/mode.h"
#include "messages/nick.h"
#include "messages/notice.h"
#include "messages/numeric.h"
#include "messages/quit.h"
#include "messages/part.h"
#include "messages/ping.h"
#include "messages/privmsg.h"

#include "lib/ansi.h"

namespace pingpong {
	std::string irc::default_nick = "pingpong";
	std::string irc::default_user = "pingpong";
	std::string irc::default_realname = "PingPong IRC";

	irc::~irc() {
		for (const std::pair<std::string, server *> &server_pair: servers)
			delete server_pair.second;
	}

	server * irc::get_server(const std::string &id) const {
		if (servers.count(id) == 0)
			return nullptr;

		return servers.at(id);
	}

	bool irc::has_server(const std::string &id) const {
		return servers.count(id) != 0;
	}

	bool irc::has_server(server *serv) const {
		return !get_key(serv).empty();
	}

	std::string irc::get_key(server *serv) const {
		for (const std::pair<std::string, server *> &server_pair: servers) {
			if (server_pair.second == serv)
				return server_pair.first;
		}

		return "";
	}

	void irc::init() {
		init_messages();
	}

	void irc::init_messages() {
		message::add_ctor<topic_message>();
		message::add_ctor<error_message>();
		message::add_ctor<join_message>();
		message::add_ctor<kick_message>();
		message::add_ctor<mode_message>();
		message::add_ctor<nick_message>();
		message::add_ctor<notice_message>();
		message::add_ctor<numeric_message>();
		message::add_ctor<quit_message>();
		message::add_ctor<part_message>();
		message::add_ctor<ping_message>();
		message::add_ctor<privmsg_message>();
	}

	std::string irc::create_id(const std::string &hostname) {
		if (!has_server(hostname))
			return hostname;

		std::string next_name;
		for (int suffix = 2;; ++suffix) {
			next_name = hostname + std::to_string(suffix);
			if (!has_server(next_name))
				return next_name;
		}
	}

	irc & irc::operator+=(server *serv) {
		if (!has_server(serv)) {
			std::string id = create_id(serv->hostname);
			serv->id = id;
			servers.insert({id, serv});
			server_order.push_back(serv);
			if (!active_server)
				active_server = serv;
			events::dispatch<server_status_event>(serv);
		}

		return *this;
	}

	irc & irc::operator-=(server *serv) {
		if (has_server(serv)) {
			servers.erase(get_key(serv));
			server_order.remove(serv);
		}

		return *this;
	}
}
