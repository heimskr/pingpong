#ifndef MESSAGES_NOTICE_H_
#define MESSAGES_NOTICE_H_

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class notice_message: public sourced_message {
		public:
			using sourced_message::sourced_message;

			static constexpr auto get_name = []() -> std::string { return "NOTICE"; };
			virtual std::string name() const override { return get_name(); }

			operator std::string() const override;
	};
}

#endif
