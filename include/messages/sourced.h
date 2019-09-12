#ifndef MESSAGES_SOURCED_H_
#define MESSAGES_SOURCED_H_

#include <string>

#include "message.h"

namespace pingpong {
	template <typename S, typename C>
	class sourced_message: public message {
		friend class triple_message;

		private:
			static std::pair<std::string, std::string> parse_source_and_content(const std::string &);

		public:
			S source;
			C content;

			sourced_message(const pingpong::line &line_, const S &source_, const C &content_):
				message(line_), source(source_), content(content_) {}
			
			sourced_message(const pingpong::line &line_, const std::string &combined);
			sourced_message(const pingpong::line &line_): sourced_message(line_, line_.parameters) {}

			// static sourced_message<std::string, std::string> combined(const pingpong::line &, const std::string & = "");
	};
}

#endif
