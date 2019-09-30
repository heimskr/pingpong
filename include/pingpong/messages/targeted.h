#ifndef MESSAGES_TARGETED_H_
#define MESSAGES_TARGETED_H_

#include <string>

#include "message.h"

namespace pingpong {
	/**
	 * Used for messages that are done by one user to another user within a channel, such as kicks.
	 */
	class targeted_message: public message {
		public:
			std::shared_ptr<channel> chan = nullptr;
			std::shared_ptr<user> who = nullptr;
			std::shared_ptr<user> whom = nullptr;
			std::string content;

			targeted_message(const pingpong::line &line_);
	};
}

#endif
