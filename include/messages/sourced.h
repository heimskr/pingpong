#ifndef MESSAGES_SOURCED_H_
#define MESSAGES_SOURCED_H_

#include <string>

#include "message.h"

namespace pingpong {
	class sourced_message: public message {
		public:
			std::string source, content;

			sourced_message(pingpong::line line_, std::string source_, std::string content):
				message(line_), source(source_), content(content) {}
			sourced_message(pingpong::line line_, std::string combined);
			sourced_message(pingpong::line line_): sourced_message(line_, line_.parameters) {}
	};
}

#endif
