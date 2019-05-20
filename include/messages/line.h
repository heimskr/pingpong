#ifndef MESSAGES_LINE_H_
#define MESSAGES_LINE_H_

#include <string>

#include "core/mask.h"

namespace pingpong {
	struct line {
		const std::string original;
		std::string tags, command, parameters;
		mask source;

		line(std::string in);

		operator std::string() const;
	};
}

#endif
