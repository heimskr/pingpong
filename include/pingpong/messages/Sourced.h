#ifndef MESSAGES_SOURCED_H_
#define MESSAGES_SOURCED_H_

#include <string>

#include "pingpong/core/Local.h"
#include "Message.h"

namespace PingPong {
	class SourcedMessage: public Message, public Local {
		public:
			Server *server;
			std::shared_ptr<User> who;
			std::string content;

			SourcedMessage(const PingPong::Line &line_);

			/** If the source of this message is a private conversation between two users, this returns a pointer to
			 *  you. */
			using Local::isUser;
	};
}

#endif
