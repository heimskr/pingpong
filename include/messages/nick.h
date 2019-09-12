#ifndef PINGPONG_MESSAGES_NICK_H_
#define PINGPONG_MESSAGES_NICK_H_

#include "messages/message.h"
#include "messages/basic.h"

namespace pingpong {
	class nick_message: public basic_message {
		public:
			using basic_message::basic_message;
			static constexpr auto get_name = []() -> std::string { return "NICK"; };

			operator std::string() const override;
			void operator()(server_ptr) const override;
	};
}

#endif
