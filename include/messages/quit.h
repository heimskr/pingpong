#ifndef MESSAGES_QUIT_H_
#define MESSAGES_QUIT_H_

#include <string>

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class quit_message: public sourced_message {
		public:
			using sourced_message::sourced_message;

			static constexpr auto get_name = []() -> std::string { return "QUIT"; };
			virtual std::string name() const override { return get_name(); }

			operator std::string() const override;
			bool operator()(server_ptr) override;
	};
}

#endif
