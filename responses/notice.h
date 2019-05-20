#ifndef RESPONSES_NOTICE_H_
#define RESPONSES_NOTICE_H_

#include <string>

#include "response.h"
#include "sourced.h"

namespace pingpong {
	class notice_response: public sourced_response {
		public:
			notice_response(pingpong::line line_, std::string source_, std::string message_):
				sourced_response(line_, source_, message_) {}
			notice_response(pingpong::line line_, std::string combined): sourced_response(line_, combined) {}
			notice_response(pingpong::line line_): sourced_response(line_) {}

			virtual std::string get_command() const override;

			operator std::string() const override;
	};
}

#endif
