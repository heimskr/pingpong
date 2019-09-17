#ifndef MESSAGES_SOURCED_H_
#define MESSAGES_SOURCED_H_

#include <string>

#include "message.h"

namespace pingpong {
	class sourced_message: public message {
		public:
			std::shared_ptr<channel> chan = nullptr;
			std::shared_ptr<user> who = nullptr;
			std::string content;

			sourced_message(const pingpong::line &line_);
	};
}

#endif
