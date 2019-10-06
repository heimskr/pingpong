#ifndef PINGPONG_MESSAGES_CAP_H_
#define PINGPONG_MESSAGES_CAP_H_

#include "pingpong/messages/message.h"

namespace pingpong {
	class cap_message: public message {
		private:
			/** Handles an ACK subcommand. */
			void ack(server *, const std::vector<std::string> &caps);

		public:
			using message::message;
			static constexpr auto get_name = []() -> std::string { return "CAP"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
