#ifndef MESSAGES_NUMERIC_H_
#define MESSAGES_NUMERIC_H_

#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_set>

#include "pingpong/core/channel.h"
#include "pingpong/core/hats.h"
#include "pingpong/core/numeric_type.h"
#include "pingpong/messages/line.h"
#include "pingpong/messages/message.h"
#include "pingpong/messages/sourced.h"

namespace pingpong {
	class numeric_message: public message {
		using names = std::tuple<std::string, channel::visibility, std::vector<std::pair<hat_set, std::string>>>;

		private:
			static std::unordered_set<numeric_type> types;

			inline int to_int() const { return static_cast<int>(type); }
			inline std::string to_string() const { return std::to_string(to_int()); }

			static names parse_names_reply(const std::string &);

			// Note: in methods like parse_ssc/parse_ss, the first word will be your nickname—that seems to be common to
			// all (or most?) numeric messages.

			/** Parses a message of the form "word word text" (space + space). */
			static std::tuple<std::string, std::string, std::string> parse_ss(const std::string &);
			static std::tuple<std::string, std::string, std::string> parse_ss(const pingpong::line &line_) {
				return parse_ss(line_.parameters);
			}

			/** Parses a message of the form "word word :text" (space + space-colon). */
			static std::tuple<std::string, std::string, std::string> parse_ssc(const std::string &);
			static std::tuple<std::string, std::string, std::string> parse_ssc(const pingpong::line &line_) {
				return parse_ssc(line_.parameters);
			}

		public:
			numeric_type type;

			numeric_message(pingpong::line);

			static constexpr auto get_name = []() -> std::string { return "_NUMERIC"; };

			operator std::string() const override;
			bool operator()(server *) override;

			/** Returns whether the numeric message type is recognized. */
			bool is_known() const;

			static bool is_numeric(const char *);
			static bool is_numeric(const char *, int &);

			bool operator==(int n) const;
			bool operator!=(int n) const;
			bool operator==(numeric_type type_) const;
			bool operator!=(numeric_type type_) const;

			bool handle_welcome(server *);       // 001
			bool handle_whois_user(server *);    // 311
			bool handle_whois_server(server *);  // 312
			bool handle_whois_idle(server *);    // 317
			bool handle_channel_modes(server *); // 324
			bool handle_channel_topic(server *); // 332
			bool handle_names_reply(server *);   // 353
	};
}

#endif
