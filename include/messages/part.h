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

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif
