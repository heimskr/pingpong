#ifndef MESSAGES_KICK_H_
#define MESSAGES_KICK_H_

#include <string>

#include "message.h"
#include "targeted.h"
#include "line.h"

namespace pingpong {
	class kick_message: public targeted_message {
		public:
			using targeted_message::targeted_message;

			static constexpr auto get_name = []() -> std::string { return "KICK"; };

			operator std::string() const override;
			bool operator()(server_ptr) override;
	};
}

#endif
