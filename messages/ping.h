#ifndef MESSAGES_PING_H_
#define MESSAGES_PING_H_

#include <string>

#include "basic.h"
#include "message.h"

namespace pingpong {
	class ping_message: public basic_message {
		public:
			ping_message(pingpong::line line_): basic_message(line_) {}

			virtual std::string get_command() const override;
			operator std::string() const override;
	};
}


#endif
