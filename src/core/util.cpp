#include <cctype>
#include <cstdlib>
#include <sstream>

#include "pingpong/core/util.h"

#include "lib/formicine/ansi.h"

namespace pingpong {
	std::array<std::string, 100> util::irc_colors = { // Nabbed straight from irssi.
		"97", "30", "34", "32", "91", "31", "35", "33", "93", "92", "36", "96", "94", "95", "90", "37", "38;5;52",
		"38;5;94",  "38;5;100", "38;5;58",  "38;5;22", "38;5;29", "38;5;23", "38;5;24", "38;5;17", "38;5;54", "38;5;53",
		"38;5;89",  "38;5;88",  "38;5;130", "38;5;142", "38;5;64",  "38;5;28",  "38;5;35",  "38;5;30",  "38;5;25",
		"38;5;18",  "38;5;91",  "38;5;90",  "38;5;125", "38;5;124", "38;5;166", "38;5;184", "38;5;106", "38;5;34",
		"38;5;49",  "38;5;37",  "38;5;33",  "38;5;19",  "38;5;129", "38;5;127", "38;5;161", "38;5;196", "38;5;208",
		"38;5;226", "38;5;154", "38;5;46",  "38;5;86",  "38;5;51",  "38;5;75",  "38;5;21",  "38;5;171", "38;5;201",
		"38;5;198", "38;5;203", "38;5;215", "38;5;227", "38;5;191", "38;5;83",  "38;5;122", "38;5;87",  "38;5;111",
		"38;5;63",  "38;5;177", "38;5;207", "38;5;205", "38;5;217", "38;5;223", "38;5;229", "38;5;193", "38;5;157",
		"38;5;158", "38;5;159", "38;5;153", "38;5;147", "38;5;183", "38;5;219", "38;5;212", "38;5;16",  "38;5;233",
		"38;5;235", "38;5;237", "38;5;239", "38;5;241", "38;5;244", "38;5;247", "38;5;250", "38;5;254", "38;5;231", ""
	};

	util::timetype util::now() {
		return std::chrono::duration_cast<timetype>(system_now());
	}

	std::chrono::system_clock::duration util::system_now() {
		return std::chrono::system_clock::now().time_since_epoch();
	}

	long util::timestamp() {
		return now().count();
	}

	long util::seconds() {
		return std::chrono::duration_cast<std::chrono::seconds>(now()).count();
	}

	long util::millistamp() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(now()).count();
	}

	long util::microstamp() {
		return std::chrono::duration_cast<std::chrono::microseconds>(now()).count();
	}

	long util::nanostamp() {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(now()).count();
	}

	util::timetype util::from_seconds(long seconds) {
		return std::chrono::duration_cast<timetype>(std::chrono::seconds(seconds));
	}

	std::string util::get_date(timetype when) {
		return format_time<64>(when, "%Y/%m/%d");
	}

	std::string util::get_date(long stamp) {
		return get_date(timetype(stamp));
	}

	std::string util::get_time(timetype when) {
		return format_time<64>(when, "%H:%M:%S");
	}

	std::string util::get_time(long stamp) {
		return get_time(timetype(stamp));
	}

	bool util::is_valid_nick(const std::string &str) {
		return !str.empty() && str.find_first_not_of(nick_chars) == std::string::npos;
	}

	std::string & util::rtrim(std::string &str) {
		str.erase(std::find_if(str.rbegin(), str.rend(), [](char c) {
			return !std::isspace(c) && c != '\n' && c != '\r' && c != '\t';
		}).base(), str.end());
		return str;
	}

	std::string util::rtrim(const std::string &str) {
		std::string copy = str;
		return rtrim(copy);
	}

	std::string util::irc2ansi(std::string str) {
		// TODO: Unicode support
		std::ostringstream out;
		const size_t length = str.length();
		str += "\0\0\0\0\0"; // 🤫

		bool bold = false, underline = false, italics = false, inverse = false;

		for (size_t i = 0; i < length; ++i) {
			char ch = str[i];
			if (ch == '\x03') {
				out << ansi::reset_fg << ansi::reset_bg;
				const std::string next = str.substr(i + 1, 5);
				std::string first = util::take_while(next, [](char c) { return std::isdigit(c); });
				if (2 < first.length())
					first.erase(2);
				std::string second;
				const size_t comma = next.find(',');
				if (comma <= 2)
					second = util::take_while(next.substr(comma + 1), [](char c) { return std::isdigit(c); });

				if (first.empty())
					continue;

				long first_long = strtol(first.c_str(), nullptr, 10), second_long = -1;
				if (!second.empty())
					second_long = strtol(second.c_str(), nullptr, 10);

				constexpr long irc_colors_length = irc_colors.size();
				if (irc_colors_length <= first_long || irc_colors_length <= second_long)
					continue;

				DBG("first_long[" << first_long << "]");
				if (!irc_colors[first_long].empty())
					out << "\x1b[" << irc_colors[first_long] << "m";

				if (-1 < second_long && !irc_colors[second_long].empty())
					out << "\x1b[" << irc_colors[second_long] << "m";

				i += first.length() + (comma <= 2? 1 + second.length() : 0);
			} else if (ch == '\x02') {
				out << (bold? ansi::style_resets.at(ansi::style::bold) :
				              ansi::style_codes.at(ansi::style::bold));
				bold = !bold;
			} else if (ch == '\x16') {
				out << (inverse? ansi::style_resets.at(ansi::style::inverse) :
				                 ansi::style_codes.at(ansi::style::inverse));
				inverse = !inverse;
			} else if (ch == '\x1d') {
				out << (italics? ansi::style_resets.at(ansi::style::italic) :
				                 ansi::style_codes.at(ansi::style::italic));
				italics = !italics;
			} else if (ch == '\x1f') {
				out << (underline? ansi::style_resets.at(ansi::style::underline) :
				                   ansi::style_codes.at(ansi::style::underline));
				underline = !underline;
			} else {
				out << ch;
			}
		}

		out << "\e[0m";
		return out.str();
	}
}
