#ifndef PINGPONG_MESSAGES_NICK_H_
#define PINGPONG_MESSAGES_NICK_H_

#include "pingpong/messages/Message.h"
#include "pingpong/messages/Sourced.h"

namespace PingPong {
	class NickMessage: public SourcedMessage {
		public:
			using SourcedMessage::SourcedMessage;

			static constexpr auto getName = []() -> std::string { return "NICK"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
