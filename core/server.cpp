#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/StreamCopier.h"

#include "server.h"

namespace pingpong {
	std::string server::to_string() const {
		return port != irc::default_port? hostname + ":" + std::to_string(port) : hostname;
	}

	bool server::start() {
		std::unique_lock<std::mutex> ulock(status_mutex);

		if (status == dead)        cleanup(ulock);
		if (status != unconnected) throw std::runtime_error("Can't connect: server not unconnected");

		return true;
	}

	void server::cleanup() {
		std::unique_lock ulock(status_mutex);
		cleanup(ulock);
	}

	void server::cleanup(std::unique_lock<std::mutex> &) {
		std::cerr << "[" << to_string() << ": cleanup]" << std::endl;
		status = unconnected;
	}
}
