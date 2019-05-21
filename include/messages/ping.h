#ifndef MESSAGES_PING_H_
#define MESSAGES_PING_H_

#include <string>

#include "basic.h"
#include "message.h"

namespace pingpong {
	class ping_message: public basic_message {
		public:
			static constexpr auto get_name = []() -> std::string { return "PING"; };

			ping_message(pingpong::line line_): basic_message(line_) {}

			operator std::string() const override;
	};
}


#endif
