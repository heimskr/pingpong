#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "Poco/StreamCopier.h"

#include "lib/ansi.h"
#include "core/debug.h"
#include "core/server.h"
#include "messages/all.h"
#include "commands/user.h"
#include "commands/nick.h"
#include "commands/pong.h"
#include "events/bad_line.h"
#include "events/event.h"
#include "events/message.h"
#include "events/raw.h"

namespace pingpong {
	server::operator std::string() const {
		return port != irc::default_port? hostname + ":" + std::to_string(port) : hostname;
	}

	bool server::start() {
		std::unique_lock<std::mutex> ulock(status_mux);

		if (status == dead)        cleanup(ulock);
		if (status != unconnected) throw std::runtime_error("Can't connect: server not unconnected");

		SocketAddress addr(hostname, port);
		socket = StreamSocket(addr);
		stream = std::make_shared<SocketStream>(socket);
		worker = std::make_shared<std::thread>(&server::work, this);

		return true;
	}

	void server::work() {
		user_command(this, parent->username, parent->realname).send();

		std::string line;
		while (std::getline(*stream, line)) {
			if (line.back() == '\r') {
				// Remove the carriage return. It's part of the spec, but std::getline removes only the newline.
				line.pop_back();
			}

			try {
				handle_line(pingpong::line(line));
			} catch (std::invalid_argument &) {
				// Already dealt with by dispatching a bad_line_event.
			}
		}
	}

	void server::handle_line(const pingpong::line &line) {
		message_ptr msg;
		try {
			msg = pingpong::message::parse(line);
		} catch (std::invalid_argument &err) {
			events::dispatch<bad_line_event>(this, line.original);
			throw;
		}

		events::dispatch<raw_in_event>(this, line.original);

		if (ping_message *ping = dynamic_cast<ping_message *>(msg.get())) {
			pong_command(this, ping->content).send();
		}

		events::dispatch<message_event>(this, msg);
		(*msg)(this);
		last_message = msg;
	}

	server & server::operator+=(const std::string &chan) {
		if (!has_channel(chan)) {
			parent->dbgout() << "Adding channel " << chan << "\r\n";
			channel_ptr cptr = std::make_shared<channel>(chan, this);
			if (channels.empty())
				active_channel = cptr;
			channels[chan] = cptr;
		} else {
			YIKES("Channel already exists: " << chan << "\r\n");
		}

		return *this;
	}

	server & server::operator-=(const std::string &chan) {
		if (has_channel(chan)) {
			parent->dbgout() << "Removing channel " << chan << "\n";
			channels.erase(chan);
			if (active_channel && active_channel->name == chan) {
				if (channels.empty())
					active_channel = nullptr;
				else
					active_channel = channels.begin()->second;
			}
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

		*stream << str << "\r\n";
		stream->flush();
	}

	void server::set_nick(const std::string &new_nick) {
		if (nick.empty())
			nick = new_nick;
		nick_command(this, new_nick).send();
	}

	bool server::has_channel(const std::string &chanstr) const {
		return channels.count(chanstr) != 0;
	}

	channel_ptr server::get_channel(const std::string &chanstr) const {
		if (!has_channel(chanstr)) return nullptr;
		return channels.at(chanstr);
	}

	user_ptr server::get_user(const std::string &name, bool create) {
		if (users.count(name) == 0)
			return create? users[name] = std::make_shared<user>(name, this) : nullptr;
		return users[name];
	}

	void server::rename_user(const std::string &old_nick, const std::string &new_nick) {
		if (old_nick == nick)
			nick = new_nick;

		if (user_ptr uptr = get_user(old_nick, false))
			uptr->name = new_nick;

		for (auto [chanstr, chan]: channels)
			chan->rename_user(old_nick, new_nick);
	}

	void server::cleanup() {
		std::unique_lock ulock(status_mux);
		cleanup(ulock);
	}

	void server::cleanup(std::unique_lock<std::mutex> &) {
		*parent << "["_d << std::string(*this) << ": cleanup]"_d << "\r\n";
		status = unconnected;

		if (worker)
			worker->join();
	}
}
