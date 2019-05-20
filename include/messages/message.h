#ifndef MESSAGES_MESSAGE_H_
#define MESSAGES_MESSAGE_H_

#include <memory>
#include <string>

#include "line.h"

namespace pingpong {
	class message;
	using message_ptr = std::unique_ptr<pingpong::message>;

	class message {
		protected:
			line line;

		public:
			message(pingpong::line line_): line(line_) {}
			virtual ~message() = 0;

			virtual std::string get_command() const = 0;
			virtual operator std::string() const;

			static message_ptr parse(const std::string &);
			static message_ptr parse(const pingpong::line &);
	};
}

#endif
