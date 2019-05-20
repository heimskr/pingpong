#ifndef MESSAGES_NOTICE_H_
#define MESSAGES_NOTICE_H_

#include <string>

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class notice_message: public sourced_message {
		public:
			notice_message(pingpong::line line_, std::string source_, std::string content):
				sourced_message(line_, source_, content) {}
			notice_message(pingpong::line line_, std::string combined): sourced_message(line_, combined) {}
			notice_message(pingpong::line line_): sourced_message(line_) {}

			virtual std::string get_command() const override;
			operator std::string() const override;
	};
}

#endif
