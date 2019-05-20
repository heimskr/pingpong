#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "Poco/StreamCopier.h"

#include "server.h"
#include "responses/all.h"

namespace pingpong {
	using std::endl, std::cout;

	server::operator std::string() const {
		return port != irc::default_port? hostname + ":" + std::to_string(port) : hostname;
	}

	bool server::start() {
		std::unique_lock<std::mutex> ulock(status_mutex);

		if (status == dead)        cleanup(ulock);
		if (status != unconnected) throw std::runtime_error("Can't connect: server not unconnected");

		SocketAddress addr(hostname, port);
		socket = StreamSocket(addr);
		stream = std::make_shared<SocketStream>(socket);
		server_thread = std::make_shared<std::thread>(&server::work, this);

		return true;
	}

	void server::work() {
		std::string line;
		while (std::getline(*stream, line)) {
			if (line.back() == '\r') {
				// Remove the carriage return.
				line.pop_back();
			}

			process_line(pingpong::line(line));
		}
	}

	void server::process_line(const pingpong::line &line) {
		response_ptr resp = pingpong::response::parse(line);
		std::cout << "Response: " << std::string(*resp) << std::endl;
	}


	void server::cleanup() {
		std::unique_lock ulock(status_mutex);
		cleanup(ulock);
	}

	void server::cleanup(std::unique_lock<std::mutex> &) {
		std::cerr << "[" << std::string(*this) << ": cleanup]" << std::endl;
		status = unconnected;

		if (server_thread) {
			server_thread->join();
		}
	}
}
