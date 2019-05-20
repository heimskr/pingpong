#ifndef RESPONSES_LINE_H_
#define RESPONSES_LINE_H_

#include <string>

namespace pingpong {
	struct line {
		std::string tags, source, command, parameters;
		line(std::string in);

		operator std::string() const;
	};
}

#endif
