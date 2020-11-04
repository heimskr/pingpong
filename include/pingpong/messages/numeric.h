#ifndef MESSAGES_NUMERIC_H_
#define MESSAGES_NUMERIC_H_

#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>

#include "pingpong/core/Channel.h"
#include "pingpong/core/Hats.h"
#include "pingpong/core/NumericType.h"
#include "pingpong/messages/Line.h"
#include "pingpong/messages/Message.h"
#include "pingpong/messages/Sourced.h"

namespace PingPong {
	class NumericMessage: public Message {
		using Names = std::tuple<std::string, Channel::Visibility, std::vector<std::pair<HatSet, std::string>>>;

		private:
			static std::unordered_set<NumericType> types;

			inline int toInt() const { return static_cast<int>(type); }
			inline std::string toString() const { return std::to_string(toInt()); }

			static Names parseNamesReply(const std::string &);

			// Note: in methods like parseSSC/parseSS, the first word will be your nickname—that seems to be common to
			// all (or most?) numeric messages.

			/** Parses a message of the form "word word text" (space + space). */
			static std::tuple<std::string, std::string, std::string> parseSS(const std::string &);
			static std::tuple<std::string, std::string, std::string> parseSS(const PingPong::Line &line_) {
				return parseSS(line_.parameters);
			}

			/** Parses a message of the form "word word :text" (space + space-colon). */
			static std::tuple<std::string, std::string, std::string> parseSSC(const std::string &);
			static std::tuple<std::string, std::string, std::string> parseSSC(const PingPong::Line &line_) {
				return parseSSC(line_.parameters);
			}

		public:
			NumericType type;

			NumericMessage(const PingPong::Line &);

			static constexpr auto getName = []() -> std::string { return "_NUMERIC"; };

			operator std::string() const override;
			bool operator()(Server *) override;

			/** Returns whether the numeric message type is recognized. */
			bool isKnown() const;

			static bool isNumeric(const char *);
			static bool isNumeric(const char *, int &);

			bool operator==(int n) const;
			bool operator!=(int n) const;
			bool operator==(NumericType type_) const;
			bool operator!=(NumericType type_) const;

			bool handleWelcome(Server *);       // 001
			bool handleISupport(Server *);     // 005
			bool handleWhoisUser(Server *);    // 311
			bool handleWhoisServer(Server *);  // 312
			bool handleWhoisIdle(Server *);    // 317
			bool handleChannelModes(Server *); // 324
			bool handleChannelTopic(Server *); // 332
			bool handleNamesReply(Server *);   // 353
			bool handleMotd(Server *);          // 372
			bool handleNickInUse(Server *);   // 433
	};
}

#endif
