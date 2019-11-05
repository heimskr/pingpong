#include <string>
#include <thread>

#include <cstring>

#include "pingpong/core/irc.h"
#include "pingpong/core/server.h"

#include "pingpong/events/join.h"
#include "pingpong/events/notice.h"
#include "pingpong/events/privmsg.h"
#include "pingpong/events/server_status.h"

#include "pingpong/messages/cap.h"
#include "pingpong/messages/cap.h"
#include "pingpong/messages/topic.h"
#include "pingpong/messages/error.h"
#include "pingpong/messages/join.h"
#include "pingpong/messages/kick.h"
#include "pingpong/messages/mode.h"
#include "pingpong/messages/nick.h"
#include "pingpong/messages/notice.h"
#include "pingpong/messages/numeric.h"
#include "pingpong/messages/quit.h"
#include "pingpong/messages/part.h"
#include "pingpong/messages/ping.h"
#include "pingpong/messages/privmsg.h"

#include "pingpong/net/resolution_error.h"

#include "lib/formicine/ansi.h"
#include "lib/formicine/futil.h"

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

	std::pair<std::string, long>
	irc::connect(const std::string &where, const std::string &nick, long port, irc::connect_wrapper wrapper) {
		if (where.empty())
			throw std::invalid_argument("Server location can't be blank");

		const size_t colon = where.find(':');
		std::string hostname = where;
		if (colon != std::string::npos) {
			if (colon == 0) {
				throw std::invalid_argument("Server location can't start with a colon");
			} else if (colon == where.size() - 1) {
				throw std::invalid_argument("Server location can't end with a colon");
			} else {
				const char *raw = where.c_str();
				char *end;
				port = strtol(raw + colon + 1, &end, 10);
				hostname = where.substr(0, colon);
				if (end != raw + where.size()) {
					DBG("end - raw = " << (reinterpret_cast<long>(end) - reinterpret_cast<long>(raw)));
					throw std::invalid_argument("Port must be numeric");
				}
			}
		}

		if (!wrapper) {
			// The wrapper function is a hack to allow the caller of irc::connect() to handle exceptions inside the
			// thread.
			wrapper = irc::connect_wrapper([&](const std::function<void()> &fn) { fn(); });
		}

		// connect() is a blocking operation, and it can take a while if the host is unavailable for some reason. A
		// thread is necessary to prevent the connection attempt from holding up whatever thread called irc::connect()
		// while it waits for connect() to time out.
		std::thread([=, this]() {
			wrapper([=, this]() {
				pingpong::server *serv = new pingpong::server(this, hostname, port);
				try {
					serv->start();
					serv->set_nick(nick);
					*this += serv;
				} catch (pingpong::net::resolution_error &err) {
					delete serv;
					throw;
				}
			});
		}).detach();

		return {hostname, port};
	}

	void irc::init() {
		init_messages();
	}

	void irc::init_messages() {
		message::add_ctor<cap_message>();
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
		auto lock = lock_servers();

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
			if (active_server == serv)
				active_server = nullptr;
			serv->kill();
			delete serv;
		}

		return *this;
	}

	void irc::add_listeners() {
		events::listen<join_event>([&](join_event *ev) {
			if (ev->who->is_self())
				ev->chan->sort_users();
		});

		events::listen<notice_event>([&](notice_event *ev) {
			if (ev->is_channel())
				ev->get_channel(ev->serv)->send_to_front(ev->speaker);
		});

		events::listen<privmsg_event>([&](privmsg_event *ev) {
			if (ev->is_channel())
				ev->get_channel(ev->serv)->send_to_front(ev->speaker);
		});

	}
}
