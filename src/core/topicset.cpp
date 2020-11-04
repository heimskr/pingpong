#include "pingpong/core/TopicSet.h"
#include "pingpong/core/User.h"

namespace PingPong {
	TopicSet & TopicSet::operator=(const std::string &str) {
		if (text != str)
			text = str;
		return *this;
	}

	TopicSet & TopicSet::operator=(const std::shared_ptr<User> &user_) {
		if (modifiedBy != user_->name)
			modifiedBy = user_->name;
		return *this;
	}

	TopicSet & TopicSet::operator=(long num) {
		modified = num;
		return *this;
	}

	bool TopicSet::operator==(const std::string &str) const {
		return text == str;
	}

	bool TopicSet::operator!=(const std::string &str) const {
		return text != str;
	}	

	bool TopicSet::operator<(const std::string &str) const {
		return text < str;
	}	
}
