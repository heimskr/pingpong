#ifndef MESSAGES_PING_H_
#define MESSAGES_PING_H_

#include <string>

#include "basic.h"
#include "message.h"

namespace pingpong {
	class ping_message: public basic_message {
		public:
			ping_message(pingpong::line line_): basic_message(line_) {}

			static constexpr auto get_name = []() -> std::string { return "PING"; };
			virtual std::string name() const override { return get_name(); }

			operator std::string() const override;
	};
}


#endif
