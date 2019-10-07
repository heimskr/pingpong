#ifndef MESSAGES_JOIN_H_
#define MESSAGES_JOIN_H_

#include "pingpong/core/server.h"
#include "pingpong/messages/message.h"

namespace pingpong {
	class join_message: public message {
		public:
			server *serv;
			std::string who, chan;
			std::string accountname, realname; // if extended-join is enabled

			join_message(const pingpong::line &line_);

			static constexpr auto get_name = []() -> std::string { return "JOIN"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
