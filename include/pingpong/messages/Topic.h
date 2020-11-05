#ifndef PINGPONG_MESSAGES_TOPIC_H_
#define PINGPONG_MESSAGES_TOPIC_H_

#include "pingpong/messages/Sourced.h"

namespace PingPong {
	class TopicMessage: public SourcedMessage {
		public:
			using SourcedMessage::SourcedMessage;
			static constexpr auto getName = []() -> std::string { return "TOPIC"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
