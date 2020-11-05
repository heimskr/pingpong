#ifndef PINGPONG_MESSAGES_PING_H_
#define PINGPONG_MESSAGES_PING_H_

#include "Basic.h"

namespace PingPong {
	class PingMessage: public BasicMessage {
		public:
			PingMessage(const PingPong::Line &line_): BasicMessage(line_) {}

			static constexpr auto getName = []() -> std::string { return "PING"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}


#endif
