#ifndef MESSAGES_NOTICE_H_
#define MESSAGES_NOTICE_H_

#include "message.h"
#include "triple.h"

namespace pingpong {
	class notice_message: public triple_message {
		public:
			using triple_message::triple_message;

			static constexpr auto get_name = []() -> std::string { return "NOTICE"; };
			operator std::string() const override;
	};
}

#endif
