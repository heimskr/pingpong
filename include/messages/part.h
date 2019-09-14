#ifndef MESSAGES_PART_H_
#define MESSAGES_PART_H_

#include <string>

#include "message.h"
#include "sourced.h"
#include "line.h"

namespace pingpong {
	class part_message: public sourced_message {
		public:
			using sourced_message::sourced_message;

			static constexpr auto get_name = []() -> std::string { return "PART"; };
			virtual std::string name() const override { return get_name(); }

			operator std::string() const override;
			bool operator()(server_ptr) override;
	};
}

#endif
