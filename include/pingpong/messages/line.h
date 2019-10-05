#ifndef PINGPONG_MESSAGES_LINE_H_
#define PINGPONG_MESSAGES_LINE_H_

#include "pingpong/core/defs.h"
#include "pingpong/core/mask.h"

namespace pingpong {
	struct line {
		const std::string original;
		server *serv;
		std::string tags, command, parameters;
		mask source;

		line(server *serv = nullptr, const std::string &original_ = "");

		operator std::string() const;
		operator bool() const;
		friend std::ostream & operator<<(std::ostream &, const line &);
	};
}

#endif
