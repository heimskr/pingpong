#ifndef MESSAGES_LINE_H_
#define MESSAGES_LINE_H_

#include "core/defs.h"
#include "core/mask.h"

namespace pingpong {
	struct line {
		const std::string original;
		server_ptr serv;
		std::string tags, command, parameters;
		mask source;

		line(server_ptr serv, const std::string &);

		operator std::string() const;
	};
}

#endif
