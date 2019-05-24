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
#include "events/event.h"
#include "events/message.h"

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
		server_thread = std::make_shared<std::thread>(&server::work, this);

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

			handle_line(pingpong::line(line));
		}
	}

	void server::handle_line(const pingpong::line &line) {
		message_ptr msg;
		try {
			msg = pingpong::message::parse(line);
		} catch (std::invalid_argument &err) {
			*parent << ansi::red << " >> " << ansi::reset << line.original << "\n";
			return;
		}

		if (ping_message *ping = dynamic_cast<ping_message *>(msg.get())) {
			pong_command(this, ping->content).send();
		}

		events::dispatch<message_event>(this, msg);
		(*msg)(this);
		last_message = msg;
	}

	server & server::operator+=(const std::string &chan) {
		if (!has_channel(chan)) {
			parent->dbgout() << "Adding channel " << chan << "\n";
			channel_ptr cptr = std::make_shared<channel>(chan, this);
			if (channels.empty())
				active_channel = cptr;
			channels[chan] = cptr;
		} else {
			YIKES("Channel already exists: " << chan << "\n");
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

	void server::quote(const std::string &str, bool silent) {
		if (!stream) {
			YIKES("server::quote" >> ansi::bold << ": Stream not ready");
			throw std::runtime_error("Stream not ready");
		}

		auto l = parent->lock_console();
		if (!silent)
			parent->dbgin() << str << "\n";

		*stream << str << "\r\n";
		stream->flush();
	}

	void server::set_nick(const std::string &new_nick) {
		if (nick.empty())
			nick = new_nick;
		nick_command(this, new_nick).send();
	}

	const std::string & server::get_nick() const {
		return nick;
	}

	bool server::has_channel(const std::string &chanstr) const {
		return channels.count(chanstr) != 0;
	}

	channel_ptr server::get_channel(const std::string &chanstr) const {
		if (!has_channel(chanstr)) return nullptr;
		return channels.at(chanstr);
	}

	void server::rename_user(const std::string &old_nick, const std::string &new_nick) {
		if (old_nick == nick)
			nick = new_nick;

		for (auto [chanstr, chan]: channels)
			chan->rename_user(old_nick, new_nick);
	}

	void server::cleanup() {
		std::unique_lock ulock(status_mux);
		cleanup(ulock);
	}

	void server::cleanup(std::unique_lock<std::mutex> &) {
		*parent << "["_d << std::string(*this) << ": cleanup]"_d << "\n";
		status = unconnected;

		if (server_thread)
			server_thread->join();
	}
}
