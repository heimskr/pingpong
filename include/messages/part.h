#ifndef MESSAGES_PART_H_
#define MESSAGES_PART_H_

#include <string>

#include "message.h"
#include "sourced.h"
#include "line.h"

namespace pingpong {
	class part_message: public message {
		public:
			std::string chan, reason;

			part_message(pingpong::line line_);

			static constexpr auto get_name = []() -> std::string { return "PART"; };
			virtual std::string name() const override { return get_name(); }

			operator std::string() const override;
			void operator()(server_ptr) const override;
	};
}

#endif
