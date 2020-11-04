#include <cctype>
#include <vector>

#include "pingpong/core/Debug.h"
#include "pingpong/core/ParseError.h"

#include "pingpong/events/Event.h"
#include "pingpong/events/NamesUpdated.h"

#include "pingpong/messages/Numeric.h"

#include "lib/formicine/ansi.h"

namespace PingPong {
	NumericMessage::NumericMessage(const PingPong::Line &line_): Message(line_) {
		int uncasted = 0;
		if (!isNumeric(line_.command.c_str(), uncasted))
			throw std::invalid_argument("Expected a numeric command");

		type = static_cast<NumericType>(uncasted);
	}


// Public instance methods


	NumericMessage::operator std::string() const {
		return "[" + toString() + "] " + line.original;
	}

	bool NumericMessage::operator()(Server *server) {
		switch (type) {
			case NumericType::Welcome:      return handleWelcome(server);      // 001
			case NumericType::ISupport:     return handleISupport(server);     // 005
			case NumericType::WhoisUser:    return handleWhoisUser(server);    // 311
			case NumericType::WhoisServer:  return handleWhoisServer(server);  // 312
			case NumericType::WhoisIdle:    return handleWhoisIdle(server);    // 317
			case NumericType::ChannelModes: return handleChannelModes(server); // 324
			case NumericType::ChannelTopic: return handleChannelTopic(server); // 332
			case NumericType::NamesReply:   return handleNamesReply(server);   // 353
			case NumericType::Motd:         return handleMotd(server);         // 372
			case NumericType::NickInUse:    return handleNickInUse(server);    // 433
			default: return true;
		}
	}

	bool NumericMessage::isKnown() const {
		return 0 < types.count(type);
	}
	
	bool NumericMessage::isNumeric(const char *str) {
		if (!std::isdigit(str[0]))
			return false;

		char *ptr;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
		long number = strtol(str, &ptr, 10);
#pragma GCC diagnostic pop
		return !*ptr;
	}

	bool NumericMessage::isNumeric(const char *str, int &l) {
		if (!std::isdigit(str[0]))
			return false;

		char *ptr;
		long result = strtol(str, &ptr, 10);
		if (*ptr)
			return false;
		l = result;
		return true;
	}

	bool NumericMessage::operator==(int number) const { return toInt() == number; }
	bool NumericMessage::operator!=(int number) const { return toInt() != number; }
	bool NumericMessage::operator==(NumericType type_) const { return type == type_; }
	bool NumericMessage::operator!=(NumericType type_) const { return type != type_; }


// Private static methods


	std::tuple<std::string, std::string, std::string> NumericMessage::parseSS(const std::string &str) {
		size_t first = str.find(' ');
		size_t second = str.find(' ', first + 1);

		try {
			return {str.substr(0, first), str.substr(first + 1, second - first - 1), str.substr(second + 1)};
		} catch (const std::out_of_range &) {
			return {"", "", ""};
		}
	}

	std::tuple<std::string, std::string, std::string> NumericMessage::parseSSC(const std::string &str) {
		size_t space = str.find(' ');
		size_t space_colon = str.find(" :");
		if (space >= space_colon)
			throw ParseError(str, "Expected space before space-colon");

		try {
			return {str.substr(0, space), str.substr(space + 1, space_colon - space - 1), str.substr(space_colon + 2)};
		} catch (const std::out_of_range &) {
			return {"", "", ""};
		}
	}


// Public static fields


	std::unordered_set<NumericType> NumericMessage::types = {
		NumericType::ChannelModes,
		NumericType::ChannelTopic,
		NumericType::TopicModified,
		NumericType::NamesReply,
		NumericType::Motd,
		NumericType::MotdStart,
		NumericType::MotdEnd,
		NumericType::NoSuchNick
	};
}

/**
 * 401 you someone :No such nick/channel
 */
