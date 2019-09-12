#ifndef MESSAGES_PRIVMSG_H_
#define MESSAGES_PRIVMSG_H_

#include "message.h"
#include "triple.h"

namespace pingpong {
	class privmsg_message: public triple_message {
		public:
			using triple_message::triple_message;

			static constexpr auto get_name = []() -> std::string { return "PRIVMSG"; };
			operator std::string() const override;
	};
}

#endif
