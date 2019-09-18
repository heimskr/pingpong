#include "messages/sourced.h"

namespace pingpong {
	sourced_message::sourced_message(const pingpong::line &line_):
	message(line_), local(""), who(line_.serv->get_user(line_.source, true)) {
		serv = line_.serv;
		const std::string &raw = line_.parameters;

		if (raw.empty())
			throw std::runtime_error("Line parameters are empty");

		if (raw.front() == ':') {
			// Some messages, like JOINs, look like "JOIN :#chan". (Recall that a line's parameters string doesn't
			// include the name of the message type—the first character is ':'.) For commands like JOIN where what we're
			// categorizing here as content is actually the channel name—the corresponding message classes are
			// responsible for using the content to find the channel before clearing the content.
			content = raw.substr(1);
		} else {
			// Other messages look like "PRIVMSG #chan :Hello".
			content = raw.substr(raw.find(':') + 1);
			where   = raw.substr(0, raw.find(' '));
			if (where.empty())
				throw std::runtime_error("Source is empty in sourced_message");
		}
	}
}
