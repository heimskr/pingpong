#ifndef MESSAGES_TARGETED_H_
#define MESSAGES_TARGETED_H_

#include <string>

#include "Message.h"

namespace PingPong {
	/**
	 * Used for messages that are done by one user to another user within a channel, such as kicks.
	 */
	class TargetedMessage: public Message {
		public:
			std::shared_ptr<Channel> channel = nullptr;
			std::shared_ptr<User> who = nullptr;
			std::shared_ptr<User> whom = nullptr;
			std::string content;

			TargetedMessage(const PingPong::Line &line_);
	};
}

#endif
