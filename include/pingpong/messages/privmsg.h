#ifndef MESSAGES_PRIVMSG_H_
#define MESSAGES_PRIVMSG_H_

#include "Message.h"
#include "Sourced.h"

namespace PingPong {
	class PrivmsgMessage: public SourcedMessage {
		public:
			using SourcedMessage::SourcedMessage;

			static constexpr auto getName = []() -> std::string { return "PRIVMSG"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
