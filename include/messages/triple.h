#ifndef MESSAGES_TRIPLE_H_
#define MESSAGES_TRIPLE_H_

#include <string>

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class triple_message: public message {
		public:
			channel_ptr chan;
			user_ptr who;
			std::string content;

			triple_message(const pingpong::line &line_, channel_ptr chan_, user_ptr who_, const std::string &content_):
				message(line_), chan(chan_), who(who_), content(content_) {}

			triple_message(const pingpong::line &);

			virtual operator std::string() const;
	};
}

#endif
