#ifndef MESSAGES_JOIN_H_
#define MESSAGES_JOIN_H_

#include "pingpong/core/Server.h"
#include "pingpong/messages/Message.h"

namespace PingPong {
	class JoinMessage: public Message {
		public:
			Server *server;
			std::string who, chan;
			std::string accountName, realname; // if extended-join is enabled

			JoinMessage(const PingPong::Line &line_);

			static constexpr auto getName = []() -> std::string { return "JOIN"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
