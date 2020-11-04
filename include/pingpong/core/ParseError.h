#ifndef PINGPONG_CORE_PARSEERROR_H_
#define PINGPONG_CORE_PARSEERROR_H_

#include <stdexcept>
#include <string>

#include "lib/formicine/ansi.h"

namespace PingPong {
	struct ParseError: public std::exception {
		std::string text;
		const char *explanation;
		ParseError(const std::string &text_, const char *explanation_ = "Parsing failed"):
			text(text_), explanation(explanation_) { DBG("ParseError: \"" << text_ << "\""); }
		const char * what() const throw() { return explanation; }
	};
}

#endif
