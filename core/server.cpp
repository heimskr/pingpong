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
		std::string line;
		while (std::getline(*stream, line)) {
			if (line.back() == '\r') {
				// Remove the carriage return.
				line.pop_back();
			}

			process_line(line);
		}
	}

	void server::process_line(const std::string &line) {
		std::string tags, source, command, parameters;
		size_t length = line.size(), index = 0, old_index;

		if (line.at(index) == '@') {
			for (; line.at(index) != ' ' && index < length; ++index);
			tags = line.substr(1, index - 1);
			++index;
		}

		if (line.at(index) == ':') {
			old_index = index;
			for (; line.at(index) != ' ' && index < length; ++index);
			source = line.substr(old_index + 1, index - old_index - 1);
			++index;
		}

		old_index = index;
		for (; line.at(index) != ' ' && index < length; ++index);
		command = line.substr(old_index, index - old_index);
		++index;
		parameters = line.substr(index);

		cout << "tags(\"" << tags << "\"), source(\"" << source << "\"), command(\"" << command << "\"), "
		     << "parameters(\"" << parameters << "\")" << endl;
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
