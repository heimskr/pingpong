#ifndef RESPONSES_LINE_H_
#define RESPONSES_LINE_H_

#include <string>

namespace pingpong {
	struct line {
		const std::string original;
		std::string tags, source, command, parameters;

		line(const std::string &in);

		operator std::string() const;
	};
}

#endif
