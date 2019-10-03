#ifndef PINGPONG_MESSAGES_ERROR_H_
#define PINGPONG_MESSAGES_ERROR_H_

#include "basic.h"

namespace pingpong {
	class error_message: public basic_message {
		public:
			error_message(pingpong::line line_): basic_message(line_) {}

			static constexpr auto get_name = []() -> std::string { return "ERROR"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
