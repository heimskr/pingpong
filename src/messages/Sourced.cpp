#include "pingpong/messages/Sourced.h"

namespace PingPong {
	SourcedMessage::SourcedMessage(const PingPong::Line &line_):
	Message(line_), Local(""), who(line_.server->getUser(line_.source, true, true)) {
		server = line_.server;
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
				throw std::runtime_error("Source is empty in SourcedMessage");
		}
	}
}
