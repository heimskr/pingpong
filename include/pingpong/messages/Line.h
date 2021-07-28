#pragma once

#include <map>
#include <ostream>
#include <string>

#include "pingpong/core/Defs.h"
#include "pingpong/core/Mask.h"

namespace PingPong {
	struct Line {
		const std::string original;
		Server *server;
		std::string command, parameters;
		Mask source;
		std::map<std::string, std::string> tags;

		Line(Server *server_ = nullptr, std::string original_ = "");

		operator std::string() const;
		operator bool() const;

		std::string stringifyTags() const;
		static std::string stringifyTags(const std::map<std::string, std::string> &);
	};

	std::ostream & operator<<(std::ostream &, const Line &);
}
