#ifndef MESSAGES_NOTICE_H_
#define MESSAGES_NOTICE_H_

#include "Message.h"
#include "Sourced.h"

namespace PingPong {
	class NoticeMessage: public SourcedMessage {
		public:
			using SourcedMessage::SourcedMessage;

			static constexpr auto getName = []() -> std::string { return "NOTICE"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
