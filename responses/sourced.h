#ifndef RESPONSE_SOURCED_H_
#define RESPONSE_SOURCED_H_

#include <string>

#include "response.h"

namespace pingpong {
	class sourced_response: public response {
		public:
			std::string source, message;

			sourced_response(pingpong::line line_, std::string source_, std::string message_):
				response(line_), source(source_), message(message_) {}
			sourced_response(pingpong::line line_, std::string combined);
			sourced_response(pingpong::line line_): sourced_response(line_, line_.parameters) {}
	};
}

#endif
