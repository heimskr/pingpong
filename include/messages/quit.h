#ifndef MESSAGES_QUIT_H_
#define MESSAGES_QUIT_H_

#include <string>

#include "message.h"
#include "basic.h"

namespace pingpong {
	class quit_message: public basic_message {
		public:
			using basic_message::basic_message;
			static constexpr auto get_name = []() -> std::string { return "QUIT"; };

			operator std::string() const override;
	};
}

#endif
