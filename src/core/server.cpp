#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include <signal.h>

#include "core/debug.h"
#include "core/server.h"

#include "commands/user.h"
#include "commands/nick.h"
#include "commands/pong.h"

#include "events/bad_line.h"
#include "events/message.h"
#include "events/names_updated.h"
#include "events/raw.h"
#include "events/server_status.h"

namespace pingpong {
	server::~server() {
		cleanup();
	}

	bool server::start() {
		std::unique_lock<std::mutex> ulock(status_mux);

		if (status == stage::dead)        cleanup();
		if (status != stage::unconnected) throw std::runtime_error("Can't connect: server not unconnected");

		sock   = std::make_shared<net::sock>(hostname, port);
		sock->connect();
		buffer = std::make_shared<net::socket_buffer>(sock.get());
		stream = std::make_shared<std::iostream>(buffer.get());

		status = stage::setuser;
		events::dispatch<server_status_event>(this);

		worker = std::thread(&server::work, this);

		return true;
	}

	void server::work() {
		signal(SIGPIPE, SIG_IGN);
		user_command(this, parent->username, parent->realname).send();

		std::string line;
		while (std::getline(*stream, line)) {
			// Remove the carriage return. It's part of the spec, but std::getline removes only the newline.
			if (line.back() == '\r')
				line.pop_back();

			try {
				handle_line(pingpong::line(this, line));
			} catch (std::invalid_argument &) {
				// Already dealt with by dispatching a bad_line_event.
			}
		}
	}

	void server::handle_line(const pingpong::line &line) {
		std::shared_ptr<message> msg;
		try {
			msg = pingpong::message::parse(line);
		} catch (std::invalid_argument &err) {
			events::dispatch<bad_line_event>(this, line.original);
			throw;
		}

		events::dispatch<raw_in_event>(this, line.original);
		if (!(*msg)(this))
			events::dispatch<message_event>(this, msg);
		last_message = msg;
	}

	server & server::operator+=(const std::string &chan) {
		if (!has_channel(chan)) {
			channels.push_back(std::make_shared<channel>(chan, this));
		} else {
			YIKES("Channel already exists: " << chan << "\r\n");
		}

		return *this;
	}

	server & server::operator-=(const std::string &chan) {
		std::shared_ptr<channel> cptr = get_channel(chan, false);
		if (cptr) {
			auto iter = std::find(channels.begin(), channels.end(), cptr);
			if (iter == channels.end()) {
				DBG(ansi::color::red << "Channel pointer is inexplicably missing from server " << ansi::bold(hostname)
					<< ": " << chan);
				return *this;
			}
			
			DBG("Removing channel " << chan);
			channels.erase(iter);
		} else {
			YIKES("Channel not in list: " << chan << "\n");
		}

		return *this;
	}

	void server::quote(const std::string &str) {
		if (!stream) {
			YIKES("server::quote" >> ansi::style::bold << ": Stream not ready");
			throw std::runtime_error("Stream not ready");
		}

		auto l = parent->lock_console();
		events::dispatch<raw_out_event>(this, str);

		*stream << str + "\r\n";
		stream->flush();
	}

	void server::set_nick(const std::string &new_nick) {
		if (nick.empty())
			nick = new_nick;
		nick_command(this, new_nick).send();
	}

	std::string server::status_string() const {
		switch (status) {
			case stage::unconnected: return "unconnected";
			case stage::setuser: return "setuser";
			case stage::setnick: return "setnick";
			case stage::ready: return "ready";
			case stage::dead: return "dead";
		}

		return "unknown";
	}

	bool server::has_channel(const std::string &chanstr) const {
		for (std::shared_ptr<channel> chan: channels) {
			if (chan->name == chanstr)
				return true;
		}

		return false;
	}

	bool server::has_user(std::shared_ptr<user> user) const {
		return std::find(users.begin(), users.end(), user) != users.end();
	}

	bool server::has_user(const std::string &name) const {
		for (std::shared_ptr<user> user: users) {
			if (user->name == name)
				return true;
		}

		return false;
	}

	std::shared_ptr<channel> server::get_channel(const std::string &chanstr, bool create) {
		if (!has_channel(chanstr)) {
			if (!create)
				return nullptr;
			*this += chanstr;
		}

		for (std::shared_ptr<channel> chan: channels) {
			if (chan->name == chanstr)
				return chan;
		}

		return nullptr;
	}

	std::shared_ptr<user> server::get_user(const std::string &name, bool create) {
		if (!has_user(name)) {
			if (!create)
				return nullptr;
			std::shared_ptr<user> new_user = std::make_shared<user>(name, this);
			users.push_back(new_user);
			return new_user;
		}

		for (std::shared_ptr<user> user: users) {
			if (user->name == name)
				return user;
		}

		return nullptr;
	}

	void server::rename_user(const std::string &old_nick, const std::string &new_nick) {
		if (old_nick == nick)
			nick = new_nick;

		if (std::shared_ptr<user> uptr = get_user(old_nick, false))
			uptr->name = new_nick;

		for (std::shared_ptr<channel> chan: channels) {
			chan->rename_user(old_nick, new_nick);
			events::dispatch<names_updated_event>(chan, this);
		}
	}

	std::shared_ptr<user> server::get_self() {
		if (nick.empty())
			throw std::runtime_error("Can't get self: no nick for server");
		return get_user(nick, true);
	}

	void server::cleanup() {
		status = stage::dead;

		if (worker.joinable()) {
			buffer->close();
			worker.join();
		}
	}

	server::operator std::string() const {
		return port != irc::default_port? hostname + ":" + std::to_string(port) : hostname;
	}
}
