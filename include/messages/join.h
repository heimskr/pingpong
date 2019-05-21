#ifndef MESSAGES_JOIN_H_
#define MESSAGES_JOIN_H_

#include <string>

#include "message.h"
#include "basic.h"

namespace pingpong {
	class join_message: public basic_message {
		public:
			using basic_message::basic_message;
			static constexpr auto get_name = []() -> std::string { return "JOIN"; };

			operator std::string() const override;
	};
}

#endif
