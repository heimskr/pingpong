#ifndef PINGPONG_CORE_TOPICSET_H_
#define PINGPONG_CORE_TOPICSET_H_

#include <memory>
#include <string>

namespace PingPong {
	class User;

	struct TopicSet {
		std::string text;
		std::string modifiedBy;
		long modified;

		TopicSet(const std::string &text_ = "", const std::string &modified_by = "", long modified_ = -1):
			text(text_), modifiedBy(modified_by), modified(modified_) {}

		TopicSet & operator=(const std::string &);
		TopicSet & operator=(const std::shared_ptr<User> &);
		TopicSet & operator=(long);

		operator std::string() const { return text; }
		bool operator==(const std::string &) const;
		bool operator!=(const std::string &) const;
		bool operator<(const std::string &) const;
	};
}

#endif
