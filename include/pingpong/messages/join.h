#ifndef MESSAGES_JOIN_H_
#define MESSAGES_JOIN_H_

#include "pingpong/core/server.h"
#include "pingpong/messages/message.h"

namespace pingpong {
	class join_message: public message {
		public:
			server *serv;
			std::string who;
			std::string chan;

			join_message(const pingpong::line &line_):
				message(line_), serv(line_.serv), who(line_.source.nick), chan(line_.parameters.substr(1)) {}

			static constexpr auto get_name = []() -> std::string { return "JOIN"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
