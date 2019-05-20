#ifndef MESSAGES_BASIC_H_
#define MESSAGES_BASIC_H_

#include <string>

#include "message.h"
#include "line.h"

namespace pingpong {
	class basic_message: public message {
		public:
			std::string text;

			basic_message(pingpong::line line_): message(line_), text(line_.parameters.substr(1)) {}
	};
}

#endif
