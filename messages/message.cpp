#include <memory>
#include <stdexcept>
#include <string>

#include "messages/all.h"
#include "core/debug.h"

namespace pingpong {
	message_ptr message::parse(const pingpong::line &line) {
		if (numeric_message::is_numeric(line.command.c_str())) {
			return std::make_unique<pingpong::numeric_message>(numeric_message(line));
		} else if (line.command == "NOTICE") {
			return std::make_unique<pingpong::notice_message>(notice_message(line));
		} else if (line.command == "PING") {
			return std::make_unique<pingpong::ping_message>(ping_message(line));
		} else if (line.command == "PRIVMSG") {
			return std::make_unique<pingpong::privmsg_message>(privmsg_message(line));
		} else if (line.command == "JOIN") {
			return std::make_unique<pingpong::join_message>(join_message(line));
		} else if (line.command == "PART") {
			return std::make_unique<pingpong::part_message>(part_message(line));
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
