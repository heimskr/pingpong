#ifndef PINGPONG_MESSAGES_MODE_H_
#define PINGPONG_MESSAGES_MODE_H_

#include "pingpong/core/Local.h"
#include "pingpong/core/ModeSet.h"

#include "pingpong/messages/Message.h"

namespace PingPong {
	/**
	 * Received whenever someone changes the mode in a channel or when your usermode is changed.
	 */
	class ModeMessage: public Message, public Local {
		public:
			ModeSet modeset;

			std::shared_ptr<User> who;

			/** If the mode change is for a channel, this stores a pointer to the channel. */
			std::shared_ptr<PingPong::Channel> chan;

			ModeMessage(const PingPong::Line &);

			static constexpr auto getName = []() -> std::string { return "MODE"; };
			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
