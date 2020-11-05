#ifndef PINGPONG_MESSAGES_CAP_H_
#define PINGPONG_MESSAGES_CAP_H_

#include "pingpong/messages/Message.h"

namespace PingPong {
	class CapMessage: public Message {
		private:
			/** Handles an ACK subcommand. */
			void ack(Server *, const std::vector<std::string> &caps);

		public:
			using Message::Message;
			static constexpr auto getName = []() -> std::string { return "CAP"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
