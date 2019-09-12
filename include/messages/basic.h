#ifndef MESSAGES_BASIC_H_
#define MESSAGES_BASIC_H_

#include <string>

#include "message.h"
#include "line.h"

namespace pingpong {
	class basic_message: public message {
		public:
			std::string content;

			basic_message(const pingpong::line &line_): message(line_), content(line_.parameters.substr(1)) {}
	};
}

#endif
