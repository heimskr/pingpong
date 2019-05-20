#ifndef MESSAGES_PRIVMSG_H_
#define MESSAGES_PRIVMSG_H_

#include <string>

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class privmsg_message: public sourced_message {
		public:
			using sourced_message::sourced_message;

			virtual std::string get_command() const override;
			operator std::string() const override;
	};
}

#endif
