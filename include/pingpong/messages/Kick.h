#ifndef MESSAGES_KICK_H_
#define MESSAGES_KICK_H_

#include <string>

#include "Message.h"
#include "Targeted.h"
#include "Line.h"

namespace PingPong {
	class KickMessage: public TargetedMessage {
		public:
			using TargetedMessage::TargetedMessage;

			static constexpr auto getName = []() -> std::string { return "KICK"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
