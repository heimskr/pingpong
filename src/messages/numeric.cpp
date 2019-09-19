#include <cctype>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "core/debug.h"

#include "events/event.h"
#include "events/names_updated.h"

#include "messages/numeric.h"

#include "lib/ansi.h"

namespace pingpong {
	numeric_message::numeric_message(pingpong::line line_): message(line_) {
		int uncasted = 0;
		if (!is_numeric(line_.command.c_str(), uncasted))
			throw std::invalid_argument("Expected a numeric command");

		type = static_cast<numeric_type>(uncasted);

		if (types.count(type) == 0)
			DBG("Unknown numeric message type:" << uncasted);
	}

	numeric_message::operator std::string() const {
		return "[" + to_string() + "] " + line.original;
	}

	bool numeric_message::operator()(server *serv) {
		switch (type) {
			case numeric_type::names_reply: return handle_names_reply(serv);
			default: return true;
		}
	}
	
	bool numeric_message::is_numeric(const char *str) {
		if (!std::isdigit(str[0]))
			return false;

		char *ptr;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
		long number = strtol(str, &ptr, 10);
#pragma GCC diagnostic pop
		return !*ptr;
	}

	bool numeric_message::is_numeric(const char *str, int &l) {
		if (!std::isdigit(str[0]))
			return false;

		char *ptr;
		long result = strtol(str, &ptr, 10);
		if (*ptr)
			return false;
		l = result;
		return true;
	}

	bool numeric_message::operator==(int number) const { return to_int() == number; }
	bool numeric_message::operator!=(int number) const { return to_int() != number; }
	bool numeric_message::operator==(numeric_type type_) const { return type == type_; }
	bool numeric_message::operator!=(numeric_type type_) const { return type != type_; }

	std::unordered_set<numeric_type> numeric_message::types = {
		numeric_type::channel_mode_is,
		numeric_type::channel_topic,
		numeric_type::topic_modified,
		numeric_type::names_reply,
		numeric_type::motd,
		numeric_type::motd_start,
		numeric_type::motd_end,
		numeric_type::no_such_nick
	};
}

/**
 * 401 you someone :No such nick/channel
 */
