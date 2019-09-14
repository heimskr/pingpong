#ifndef MESSAGES_PRIVMSG_H_
#define MESSAGES_PRIVMSG_H_

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class privmsg_message: public sourced_message {
		public:
			using sourced_message::sourced_message;

			static constexpr auto get_name = []() -> std::string { return "PRIVMSG"; };

			operator std::string() const override;
			bool operator()(server_ptr) override;
	};
}

#endif
