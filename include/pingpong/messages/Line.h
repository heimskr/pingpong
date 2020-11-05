#ifndef PINGPONG_MESSAGES_LINE_H_
#define PINGPONG_MESSAGES_LINE_H_

#include "pingpong/core/Defs.h"
#include "pingpong/core/Mask.h"

namespace PingPong {
	struct Line {
		const std::string original;
		Server *server;
		std::string tags, command, parameters;
		Mask source;

		Line(Server *server_ = nullptr, const std::string &original_ = "");

		operator std::string() const;
		operator bool() const;
	};

	std::ostream & operator<<(std::ostream &, const Line &);
}

#endif
