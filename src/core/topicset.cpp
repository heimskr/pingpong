#include "pingpong/core/topicset.h"
#include "pingpong/core/user.h"

namespace pingpong {
	topicset & topicset::operator=(const std::string &str) {
		if (text != str)
			text = str;
		return *this;
	}

	topicset & topicset::operator=(const std::shared_ptr<user> &user_) {
		if (modified_by != user_->name)
			modified_by = user_->name;
		return *this;
	}

	topicset & topicset::operator=(long num) {
		modified = num;
		return *this;
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
