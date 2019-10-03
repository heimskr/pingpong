#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include <signal.h>
#include <unistd.h>

#include "pingpong/core/debug.h"
#include "pingpong/core/server.h"

#include "pingpong/commands/user.h"
#include "pingpong/commands/nick.h"
#include "pingpong/commands/pong.h"

#include "pingpong/events/bad_line.h"
#include "pingpong/events/message.h"
#include "pingpong/events/names_updated.h"
#include "pingpong/events/raw.h"
#include "pingpong/events/server_status.h"
#include "pingpong/events/user_appeared.h"

namespace pingpong {
	server::server(irc *parent_, const std::string &id_, const std::string &hostname_, int port_):
	parent(parent_), id(id_), hostname(hostname_), port(port_) {
		getline_mutex.lock();
	}


// Private instance methods


	void server::work_read() {
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

		getline_mutex.unlock();
	}

	void server::work_reap() {
		// Bad things happen if the reader tries to close the connection on its own, so it's necessary to use another
		// thread to start the process of killing a server. The line_done condition variable causes the reaper to wait
		// until the reader leaves its try block and the temporary pingpong::line's destructor is called.
		// If you were to directly call server::remove from error_message::operator(), the server would be deleted
		// before the pingpong::line's destructor could be called.
		std::unique_lock<std::mutex> death_lock {death_mutex};
		death.wait(death_lock);
		buffer->close();
		getline_mutex.lock();
		getline_mutex.unlock();
		*parent -= this;
	}

	void server::handle_line(const pingpong::line &line) {
		std::shared_ptr<message> msg;
		try {
			msg = pingpong::message::parse(line);
		} catch (std::invalid_argument &err) {
			DBG("Bad line: " << err.what());
			events::dispatch<bad_line_event>(this, line.original);
			throw;
		}

		events::dispatch<raw_in_event>(this, line.original);
		if (!(*msg)(this))
			events::dispatch<message_event>(this, msg);
		last_message = msg;
	}


// Public instance methods


	bool server::start() {
		auto lock {lock_status()};

		if (status != stage::unconnected)
			throw std::runtime_error("Can't connect: server not unconnected");

		sock   = std::make_shared<net::sock>(hostname, port);
		sock->connect();
		buffer = std::make_shared<net::socket_buffer>(sock.get());
		stream = std::make_shared<std::iostream>(buffer.get());

		const stage old_status = status;
		status = stage::setuser;
		if (status != old_status)
			events::dispatch<server_status_event>(this);

		worker = std::thread(&server::work_read, this);
		worker.detach();
		
		reaper = std::thread(&server::work_reap, this);
		reaper.detach();

		return true;
	}

	void server::quote(const std::string &str) {
		if (!stream) {
			YIKES("server::quote" >> ansi::style::bold << ": Stream not ready");
			throw std::runtime_error("Stream not ready");
		}

		auto l = parent->lock_console();
		events::dispatch<raw_out_event>(this, str);

		*stream << str << "\r\n";
		stream->flush();
	}

	void server::set_nick(const std::string &new_nick, bool immediate) {
		if (immediate) {
			nick = new_nick;
		} else {
			nick_command(this, new_nick).send();
		}
	}

	server::stage server::get_status() {
		auto lock {lock_status()};
		return status;
	}

	void server::set_status(stage new_status) {
		auto lock {lock_status()};
		if (new_status != status) {
			const stage old_status = status;
			status = new_status;
			events::dispatch<server_status_event>(this, old_status);
		}
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

	bool server::add_channel(const std::string &chan) {
		if (!has_channel(chan)) {
			channels.push_back(std::make_shared<channel>(chan, this));
			return true;
		}

		DBG("Channel already exists: " << chan << "\r\n");
		return false;
	}

	bool server::remove_channel(const std::shared_ptr<channel> &chan) {
		if (!chan)
			return false;

		auto iter = std::find(channels.begin(), channels.end(), chan);
		if (iter == channels.end()) {
			DBG(ansi::color::red << "Channel pointer is inexplicably missing from server " << ansi::bold(id) << ": "
				<< chan->name);
			return false;
		}

		DBG("Removing channel " << chan->name);
		channels.erase(iter);

		return true;
	}

	bool server::remove_channel(const std::string &chan) {
		std::shared_ptr<channel> cptr = get_channel(chan, false);
		if (cptr)
			return remove_channel(cptr);
		DBG("Channel not in list: " << chan << "\n");
		return false;
	}

	bool server::remove_user(const std::shared_ptr<user> &whom) {
		if (!whom)
			return false;

		auto iter = std::find(users.begin(), users.end(), whom);
		if (iter == users.end()) {
			DBG(ansi::color::red << "User pointer is inexplicably missing from server " << ansi::bold(id) << ": "
				<< whom);
			return false;
		}

		DBG("Removing user " << whom->name);
		users.erase(iter);

		return true;
	}

	bool server::remove_user(const std::string &whom) {
		std::shared_ptr<user> uptr = get_user(whom, false);
		if (uptr)
			return remove_user(uptr);
		DBG("User not in list: " << whom << "\n");
		return false;
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
			add_channel(chanstr);
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
			events::dispatch<user_appeared_event>(new_user);
			return new_user;
		}

		for (std::shared_ptr<user> user: users) {
			if (user->name == name)
				return user;
		}

		return nullptr;
	}

	std::shared_ptr<user> server::get_user(const mask &mask_, bool create) {
		if (mask_.is_server()) {
			// If the host and user are empty, then it's presumably a message from the server (it'll look like
			// {nick="irc.example.com", user="", host=""}) and we return nullptr because there's no actual user here.
			return nullptr;
		}

		// Otherwise, pass it off to another overload.
		return get_user(mask_.nick, create);
	}


	void server::rename_user(const std::string &old_nick, const std::string &new_nick) {
		if (old_nick == nick) {
			nick = new_nick;
			remove_user("?");
		}

		if (std::shared_ptr<user> uptr = get_user(old_nick, false))
			uptr->name = new_nick;

		for (std::shared_ptr<channel> chan: channels) {
			chan->rename_user(old_nick, new_nick);
			events::dispatch<names_updated_event>(chan);
		}
	}

	void server::sort_channels() {
		channels.sort([&](std::weak_ptr<channel> left, std::weak_ptr<channel> right) -> bool {
			return left.lock()->name < right.lock()->name;
		});
	}

	bool server::is_active() const {
		return status != stage::dead && status != stage::unconnected;
	}

	std::shared_ptr<user> server::get_self() {
		// If you don't have a nick yet, return a fake "?" user. Once you do have a nick, the fake user is removed.
		return get_user(nick.empty()? "?" : nick, true);
	}

	void server::kill() {
		auto lock {lock_status()};
		if (status == stage::dead)
			return;

		buffer->close();
		set_dead();
	}

	void server::reap() {
		death.notify_all();
	}

	void server::set_dead() {
		auto lock {lock_status()};
		if (status == stage::dead)
			return;
		const stage old_status = status;
		status = stage::dead;
		events::dispatch<server_status_event>(this, old_status);
	}

	server::operator std::string() const {
		return port != irc::default_port? hostname + ":" + std::to_string(port) : hostname;
	}

	std::unique_lock<std::recursive_mutex> server::lock_status() {
		return std::unique_lock(status_mutex);
	}
}
