#ifndef PINGPONG_CORE_PARSE_ERROR_H_
#define PINGPONG_CORE_PARSE_ERROR_H_

#include <stdexcept>
#include <string>

#include "lib/formicine/ansi.h"

namespace pingpong {
	struct parse_error: public std::exception {
		std::string text;
		const char *explanation;
		parse_error(const std::string &text_, const char *explanation_ = "Parsing failed"):
			text(text_), explanation(explanation_) { DBG("parse_error: \"" << text_ << "\""); }
		const char * what() const throw() { return explanation; }
	};
}

#endif
