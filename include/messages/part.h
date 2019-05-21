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

			virtual std::string get_command() const override;
			operator std::string() const override;
	};
}

#endif
