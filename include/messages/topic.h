#ifndef PINGPONG_MESSAGES_TOPIC_H_
#define PINGPONG_MESSAGES_TOPIC_H_

#include "messages/sourced.h"

namespace pingpong {
	class topic_message: public sourced_message {
		public:
			using sourced_message::sourced_message;
			static constexpr auto get_name = []() -> std::string { return "TOPIC"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
