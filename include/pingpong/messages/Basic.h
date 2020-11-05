#ifndef MESSAGES_BASIC_H_
#define MESSAGES_BASIC_H_

#include <string>

#include "Message.h"
#include "Line.h"

namespace PingPong {
	class BasicMessage: public Message {
		public:
			std::string content;

			BasicMessage(const PingPong::Line &line_): Message(line_), content(line_.parameters.substr(1)) {}
	};
}

#endif
