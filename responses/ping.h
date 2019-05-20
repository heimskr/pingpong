#ifndef RESPONSES_PING_H_
#define RESPONSES_PING_H_

#include <string>

#include "basic.h"
#include "response.h"

namespace pingpong {
	class ping_response: public basic_response {
		public:
			ping_response(pingpong::line line_): basic_response(line_) {}

			virtual std::string get_command() const override;
			operator std::string() const override;
	};
}


#endif
