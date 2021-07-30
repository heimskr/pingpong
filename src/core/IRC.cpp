#include <cstring>
#include <string>
#include <thread>

#include "pingpong/commands/User.h"

#include "pingpong/core/IRC.h"
#include "pingpong/core/Server.h"

#include "pingpong/events/Join.h"
#include "pingpong/events/Notice.h"
#include "pingpong/events/Privmsg.h"
#include "pingpong/events/ServerStatus.h"

#include "pingpong/messages/Cap.h"
#include "pingpong/messages/Topic.h"
#include "pingpong/messages/Error.h"
#include "pingpong/messages/Join.h"
#include "pingpong/messages/Kick.h"
#include "pingpong/messages/Mode.h"
#include "pingpong/messages/Nick.h"
#include "pingpong/messages/Notice.h"
#include "pingpong/messages/Numeric.h"
#include "pingpong/messages/Quit.h"
#include "pingpong/messages/Part.h"
#include "pingpong/messages/Ping.h"
#include "pingpong/messages/Privmsg.h"

#include "pingpong/net/ResolutionError.h"

#include "lib/formicine/ansi.h"
#include "lib/formicine/futil.h"

namespace PingPong {
	std::string IRC::defaultNick = "pingpong";
	std::string IRC::defaultUser = "pingpong";
	std::string IRC::defaultRealname = "PingPong IRC";

	IRC::~IRC() {
		// A delicate dance, featuring exquisitely pathological synchronization behavior
		std::mutex perish;
		perish.lock();
		while (!servers.empty()) {
			servers.begin()->second->reap(perish);
			perish.lock();
		}
	}

	Server * IRC::getServer(const std::string &id) const {
		if (servers.count(id) == 0)
			return nullptr;

		return servers.at(id);
	}

	bool IRC::hasServer(const std::string &id) const {
		return servers.count(id) != 0;
	}

	bool IRC::hasServer(Server *serv) const {
		return !getKey(serv).empty();
	}

	std::string IRC::getKey(Server *serv) const {
		for (const std::pair<std::string, Server *> &server_pair: servers)
			if (server_pair.second == serv)
				return server_pair.first;

		return "";
	}

	std::pair<std::string, long>
	IRC::connect(const std::string &where, const std::string &nick, long port, bool ssl, IRC::ConnectWrapper wrapper) {
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
			// The wrapper function is a hack to allow the caller of IRC::connect() to handle exceptions inside the
			// thread.
			wrapper = IRC::ConnectWrapper([&](const std::function<void()> &fn) { fn(); });
		}

		// connect() is a blocking operation, and it can take a while if the host is unavailable for some reason. A
		// thread is necessary to prevent the connection attempt from holding up whatever thread called IRC::connect()
		// while it waits for connect() to time out.
		std::thread([=, this]() {
			wrapper([=, this]() {
				PingPong::Server *server = new PingPong::Server(this, ssl, hostname, port);
				try {
					server->start();
					server->setNick(nick);
					UserCommand(server, username, realname).send();
					*this += server;
				} catch (PingPong::Net::ResolutionError &err) {
					delete server;
					throw;
				}
			});
		}).detach();

		return {hostname, port};
	}

	void IRC::init() {
		initMessages();
	}

	void IRC::initMessages() {
		Message::addConstructor<CapMessage>();
		Message::addConstructor<TopicMessage>();
		Message::addConstructor<ErrorMessage>();
		Message::addConstructor<JoinMessage>();
		Message::addConstructor<KickMessage>();
		Message::addConstructor<ModeMessage>();
		Message::addConstructor<NickMessage>();
		Message::addConstructor<NoticeMessage>();
		Message::addConstructor<NumericMessage>();
		Message::addConstructor<QuitMessage>();
		Message::addConstructor<PartMessage>();
		Message::addConstructor<PingMessage>();
		Message::addConstructor<PrivmsgMessage>();
	}

	std::string IRC::createID(const std::string &hostname) {
		if (!hasServer(hostname))
			return hostname;

		std::string next_name;
		for (int suffix = 2;; ++suffix) {
			next_name = hostname + std::to_string(suffix);
			if (!hasServer(next_name))
				return next_name;
		}
	}

	IRC & IRC::operator+=(Server *server) {
		auto lock = lockServers();

		if (!hasServer(server)) {
			std::string id = createID(server->hostname);
			server->id = id;
			servers.insert({id, server});
			serverOrder.push_back(server);
			if (!activeServer)
				activeServer = server;
			Events::dispatch<ServerStatusEvent>(server);
		}

		return *this;
	}

	IRC & IRC::operator-=(Server *server) {
		auto lock = lockServers();

		if (hasServer(server)) {
			servers.erase(getKey(server));
			serverOrder.remove(server);
			if (activeServer == server)
				activeServer = nullptr;
			server->kill();
			delete server;
		}

		return *this;
	}

	void IRC::addListeners() {
		Events::listen<JoinEvent>([&](JoinEvent *ev) {
			if (ev->who->isSelf())
				ev->channel->sortUsers();
		});

		Events::listen<NoticeEvent>([&](NoticeEvent *ev) {
			if (ev->isChannel())
				ev->getChannel(ev->server)->sendToFront(ev->speaker);
		});

		Events::listen<PrivmsgEvent>([&](PrivmsgEvent *ev) {
			if (ev->isChannel())
				ev->getChannel(ev->server)->sendToFront(ev->speaker);
		});

	}
}
