#ifndef RESPONSES_BASIC_H_
#define RESPONSES_BASIC_H_

#include <string>

#include "response.h"
#include "line.h"

namespace pingpong {
	class basic_response: public response {
		public:
			std::string text;

			basic_response(pingpong::line line_): response(line_), text(line_.parameters.substr(1)) {}
	};
}

#endif
