#ifndef PINGPONG_MESSAGES_LINE_H_
#define PINGPONG_MESSAGES_LINE_H_

#include "core/ppdefs.h"
#include "core/mask.h"

namespace pingpong {
	struct line {
		const std::string original;
		server *serv;
		std::string tags, command, parameters;
		mask source;

		line(server *serv, const std::string &);

		operator std::string() const;
	};
}

#endif
