#include "core/topicset.h"

namespace pingpong {
	topicset & topicset::operator=(const std::string &str) {
		if (text != str)
			text = str;
	}

	topicset & topicset::operator=(const std::shared_ptr<user> &user_) {
		if (modified_by != user_->name)
			modified_by = user_->name;
	}

	topicset & topicset::operator=(long num) {
		modified = num;
	}

	bool topicset::operator==(const std::string &str) const {
		return text == str;
	}

	bool topicset::operator!=(const std::string &str) const {
		return text != str;
	}	

	bool topicset::operator<(const std::string &str) const {
		return text < str;
	}	
}
