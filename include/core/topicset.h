#ifndef PINGPONG_CORE_TOPICSET_H_
#define PINGPONG_CORE_TOPICSET_H_

#include <string>

namespace pingpong {
	struct topicset {
		std::string text;
		std::string modified_by;
		long modified;

		topicset(const std::string &text_ = "", const std::string &modified_by_ = "", long modified_ = -1):
			text(text_), modified_by(modified_by_), modified(modified_) {}

		operator std::string() const { return text; }
	};
}

#endif
