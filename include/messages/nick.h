#ifndef PINGPONG_MESSAGES_NICK_H_
#define PINGPONG_MESSAGES_NICK_H_

#include "messages/message.h"
#include "messages/sourced.h"

namespace pingpong {
	class nick_message: public sourced_message {
		public:
			using sourced_message::sourced_message;

			static constexpr auto get_name = []() -> std::string { return "NICK"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
