#ifndef MESSAGES_SOURCED_H_
#define MESSAGES_SOURCED_H_

#include <string>

#include "pingpong/core/local.h"
#include "message.h"

namespace pingpong {
	class sourced_message: public message, public local {
		public:
			server *serv;
			std::shared_ptr<user> who;
			std::string content;

			sourced_message(const pingpong::line &line_);

			/** If the source of this message is a private conversation between two users, this returns a pointer to
			 *  you. */
			using local::is_user;
	};
}

#endif
