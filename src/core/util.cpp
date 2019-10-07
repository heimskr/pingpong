#include <cctype>
#include <cstdlib>
#include <sstream>

#include "pingpong/core/util.h"

#include "lib/formicine/ansi.h"

namespace pingpong {
	std::array<std::string, 16> util::irc_colors = {
		"white",
		"black",
		"blue",
		"green",
		"red",
		"brown",
		"purple",
		"orange",
		"yellow",
		"yeen",
		"cyan",
		"sky",
		"blue",
		"magenta",
		"gray",
		"lightgray"
	};

	std::chrono::system_clock::duration util::now() {
		return std::chrono::system_clock::now().time_since_epoch();
	}

	long util::timestamp() {
		return std::chrono::duration_cast<timetype>(now()).count();
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

		bool in_color = false, bold = false, underline = false, italics = false, inverse = false;

		for (size_t i = 0; i < length; ++i) {
			char ch = str[i];
			if (ch == '\x03') {
				if (!in_color) {
					std::string next = str.substr(i + 1, 5);
					std::string first = util::take_while(next, [](char c) { return std::isdigit(c); });
					std::string second;
					const size_t comma = next.find(',');
					if (comma <= 2)
						second = util::take_while(next.substr(comma + 1), [](char c) { return std::isdigit(c); });

					if (first.empty())
						continue;

					long first_long = strtol(first.c_str(), nullptr, 10), second_long = -1;
					if (!second.empty())
						second_long = strtol(second.c_str(), nullptr, 10);

					if (16 <= first_long || 16 <= second_long)
						continue;

					out << ansi::get_fg(ansi::get_color(irc_colors[first_long]));
					if (-1 < second_long)
						out << ansi::get_bg(ansi::get_color(irc_colors[second_long]));

					i += first.length() + (comma <= 2? 1 + second.length() : 0);
				} else {
					out << ansi::reset_fg << ansi::reset_bg;
				}

				in_color = !in_color;
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
