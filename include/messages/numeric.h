#ifndef MESSAGES_NUMERIC_H_
#define MESSAGES_NUMERIC_H_

#include <stdexcept>
#include <string>

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class numeric_message: public message {
		public:
			static bool is_numeric(const char *);
			static bool is_numeric(const char *, long &);

			long number;

			numeric_message(pingpong::line line_): message(line_) {
				if (!is_numeric(line_.command.c_str(), number))
					throw std::invalid_argument("Expected a numeric command");
			}

			virtual std::string get_command() const override;
			operator std::string() const override;

	};
}

#endif
