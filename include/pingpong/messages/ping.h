#ifndef PINGPONG_MESSAGES_PING_H_
#define PINGPONG_MESSAGES_PING_H_

#include "basic.h"

namespace pingpong {
	class ping_message: public basic_message {
		public:
			ping_message(pingpong::line line_): basic_message(line_) {}

			static constexpr auto get_name = []() -> std::string { return "PING"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}


#endif
