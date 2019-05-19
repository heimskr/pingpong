#ifndef RESPONSES_RESPONSE_H_
#define RESPONSES_RESPONSE_H_

#include <string>

namespace pingpong {
	struct line {
		std::string tags, source, command, parameters;
		line(std::string in);
	};
}

#endif
