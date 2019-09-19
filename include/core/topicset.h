#ifndef PINGPONG_CORE_TOPICSET_H_
#define PINGPONG_CORE_TOPICSET_H_

#include <string>

#include "core/user.h"

namespace pingpong {
	struct topicset {
		std::string text;
		std::string modified_by;
		long modified;

		topicset(const std::string &text_ = "", const std::string &modified_by_ = "", long modified_ = -1):
			text(text_), modified_by(modified_by_), modified(modified_) {}

		topicset & operator=(const std::string &);
		topicset & operator=(const std::shared_ptr<user> &);
		topicset & operator=(long);

		operator std::string() const { return text; }
		bool operator==(const std::string &) const;
		bool operator!=(const std::string &) const;
		bool operator<(const std::string &) const;
	};
}

#endif
