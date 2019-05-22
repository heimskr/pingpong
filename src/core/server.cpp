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

		if (message::is<ping_message>(msg)) {
			auto &ping = dynamic_cast<ping_message &>(*msg);
			pong_command(this, ping.text).send();
		}

		events::dispatch<message_event>(this, msg);
	}

	void server::quote(const std::string &str) {
		if (stream == nullptr) {
			YIKES("server::quote" >> ansi::bold << ": Stream not ready");
			throw std::runtime_error("Stream not ready");
		}

		auto l = parent->lock_console();
		parent->dbgin() << str << "\n";

		*stream << str << "\r\n";
		stream->flush();
	}

	void server::set_nick(const std::string &new_nick) {
		nick_command(this, new_nick).send();
	}

	const std::string & server::get_nick() const {
		return nick;
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
