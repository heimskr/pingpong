#include "messages/all.h"

namespace pingpong {
	triple_message::triple_message(const pingpong::line &line_): message(line_) {
		// DBG("parameters = \"" << line_.parameters << "\"");

		// const std::string &combined = line_.parameters;
		// size_t length = combined.size();

		// std::string channel_str, user_str, content_str;

		// size_t i;
		// for (i = 0; combined[i] != ' ' && i < length; ++i)
		// 	channel_str += combined[i];

		// for (; combined[i] == ' ' && i < length; ++i);

		// for (; combined[i] != ' ' && i < length; ++i) {
		// 	user_str

		// if (i == length || combined[i] != ':')
		// 	throw std::runtime_error("Couldn't parse triple_message");
	}
}
