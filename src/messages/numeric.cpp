#include <cctype>
#include <vector>

#include "core/debug.h"
#include "core/parse_error.h"

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
	}


// Public instance methods


	numeric_message::operator std::string() const {
		return "[" + to_string() + "] " + line.original;
	}

	bool numeric_message::operator()(server *serv) {
		switch (type) {
			case numeric_type::channel_topic: return handle_channel_topic(serv); // 332
			case numeric_type::names_reply:   return handle_names_reply(serv);   // 353
			default: return true;
		}
	}

	bool numeric_message::is_known() const {
		return 0 < types.count(type);
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


// Private static methods


	std::tuple<std::string, std::string, std::string> numeric_message::parse_ssc(const std::string &str) {
		size_t space = str.find(' ');
		size_t space_colon = str.find(" :");
		if (space >= space_colon)
			throw parse_error("Expected space before space-colon");

		return {str.substr(0, space), str.substr(space + 1, space_colon - space - 1), str.substr(space_colon + 2)};
	}


// Public static fields


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
