#ifndef MESSAGES_NICK_H_
#define MESSAGES_NICK_H_

#include <string>

#include "core/defs.h"
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
