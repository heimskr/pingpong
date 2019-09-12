#ifndef MESSAGES_SOURCED_H_
#define MESSAGES_SOURCED_H_

#include <string>

#include "message.h"

namespace pingpong {
	class sourced_message: public message {
		public:
			channel_ptr chan = nullptr;
			user_ptr who = nullptr;
			std::string content;

			sourced_message(const pingpong::line &line_);
	};
}

#endif
