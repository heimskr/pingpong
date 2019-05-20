#include <memory>
#include <stdexcept>
#include <string>

#include "messages/all.h"
#include "core/debug.h"

namespace pingpong {
	message_ptr message::parse(const pingpong::line &line) {
		if (line.command == "NOTICE") {
			return std::make_unique<pingpong::notice_message>(notice_message(line));
		} else if (line.command == "PING") {
			return std::make_unique<pingpong::ping_message>(ping_message(line));
		} else {
			throw std::invalid_argument("Unknown message");
		}
	}

	message_ptr message::parse(const std::string &text) {
		return parse(pingpong::line(text));
	}

	message::~message() {}

	message::operator std::string() const {
		return std::string(line);
	}
}
