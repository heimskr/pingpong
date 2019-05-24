#ifndef MESSAGES_PART_H_
#define MESSAGES_PART_H_

#include <string>

#include "message.h"
#include "sourced.h"
#include "line.h"

namespace pingpong {
	class part_message: public message {
		public:
			static constexpr auto get_name = []() -> std::string { return "PART"; };
			std::string chan, reason;

			part_message(pingpong::line line_);

			operator std::string() const override;
			void operator()(server_ptr) const override;
	};
}

#endif
