#ifndef MESSAGES_QUIT_H_
#define MESSAGES_QUIT_H_

#include <string>

#include "Message.h"
#include "Sourced.h"

namespace PingPong {
	class QuitMessage: public SourcedMessage {
		public:
			using SourcedMessage::SourcedMessage;

			static constexpr auto getName = []() -> std::string { return "QUIT"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
