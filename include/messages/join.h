#ifndef MESSAGES_JOIN_H_
#define MESSAGES_JOIN_H_

#include <string>

#include "core/server.h"
#include "messages/message.h"
#include "messages/sourced.h"

namespace pingpong {
	class join_message: public sourced_message<channel_ptr, user_ptr> {
		public:
			using sourced_message::sourced_message;

			static constexpr auto get_name = []() -> std::string { return "JOIN"; };

			operator std::string() const override;
			void operator()(server_ptr) const override;
	};
}

#endif
