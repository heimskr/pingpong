#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "Poco/StreamCopier.h"

#include "server.h"

namespace pingpong {
	using std::endl, std::cout;

	std::string server::to_string() const {
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
		// Poco::StreamCopier::copyStream(*stream, std::cout);
		std::string e;
		while (std::getline(*stream, e)) {

		// stream->copyfmt
		// while (*stream >> e) {
			// std::cout << "[" << std::endl << e << "]" << std::endl;
			cout << e;
			cout.flush();
			cout << "(((";
			cout.flush();
			cout << ")))";
			cout.flush();
			cout << endl << endl;
		}
	}

	void server::cleanup() {
		std::unique_lock ulock(status_mutex);
		cleanup(ulock);
	}

	void server::cleanup(std::unique_lock<std::mutex> &) {
		std::cerr << "[" << to_string() << ": cleanup]" << std::endl;
		status = unconnected;

		if (server_thread) {
			server_thread->join();
		}
	}
}
