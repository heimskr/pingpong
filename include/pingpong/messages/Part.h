#ifndef MESSAGES_PART_H_
#define MESSAGES_PART_H_

#include <string>

#include "Message.h"
#include "Sourced.h"
#include "Line.h"

namespace PingPong {
	class PartMessage: public SourcedMessage {
		public:
			using SourcedMessage::SourcedMessage;

			static constexpr auto getName = []() -> std::string { return "PART"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
