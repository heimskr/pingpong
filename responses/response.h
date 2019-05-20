#ifndef RESPONSES_RESPONSE_H_
#define RESPONSES_RESPONSE_H_

#include <memory>
#include <string>

#include "line.h"

namespace pingpong {
	class response;
	using response_ptr = std::unique_ptr<pingpong::response>;

	class response {
		private:
			pingpong::line line;

		public:
			response(pingpong::line line_): line(line_) {}
			virtual ~response() = 0;

			virtual std::string get_command() const = 0;
			virtual std::string to_string() const { return get_command(); }
			operator std::string() { return to_string(); }

			static response_ptr parse(const std::string &);
			static response_ptr parse(const pingpong::line &);
	};
}

#endif
