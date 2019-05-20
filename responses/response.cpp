#include <memory>
#include <stdexcept>
#include <string>

#include "all.h"

namespace pingpong {
	response_ptr response::parse(const pingpong::line &line) {
		if (line.command == "NOTICE") {
			return std::make_unique<pingpong::notice_response>(notice_response(line));
		} else {
			throw std::runtime_error("Unknown command");
		}
	}

	response_ptr response::parse(const std::string &text) {
		return parse(pingpong::line(text));
	}

	response::~response() {}

	std::string response::to_string() const {
		return "T[" + line.tags + "], S[" + line.source + "], C[" + line.command + "], P[" + line.parameters   + "]";
	}
}
