#ifndef MESSAGES_JOIN_H_
#define MESSAGES_JOIN_H_

#include "core/server.h"
#include "messages/message.h"
#include "messages/sourced.h"

namespace pingpong {
	class join_message: public sourced_message {
		public:
			join_message(const pingpong::line &line_);

			static constexpr auto get_name = []() -> std::string { return "JOIN"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
