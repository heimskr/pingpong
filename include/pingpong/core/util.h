#ifndef PINGPONG_CORE_UTIL_H_
#define PINGPONG_CORE_UTIL_H_

#include <array>
#include <deque>
#include <memory>
#include <chrono>
#include <string>

namespace pingpong {
	/**
	 * Contains miscellaneous utility functions for pingpong.
	 */
	struct util {

		static constexpr const char *nick_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
			"-_[]{}\\`|";
		static constexpr const char *flag_chars = "+-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		static std::array<std::string, 16> irc_colors;

		using timetype = std::chrono::microseconds;

		static timetype now();
		static std::chrono::system_clock::duration system_now();

		static constexpr long precision = 1e6;


		static long timestamp();
		static long seconds();
		static long millistamp();
		static long microstamp();
		static long nanostamp();

		static timetype from_seconds(long);

		template <size_t Size = 64>
		static std::string format_time(timetype when, const char *format) {
			std::chrono::system_clock::time_point tpoint {when};
			std::time_t time = std::chrono::system_clock::to_time_t(tpoint);
			char str[Size];
			std::strftime(str, sizeof(str), format, std::localtime(&time));
			return str;
		}

		template <size_t Size = 64>
		static std::string format_time(long stamp, const char *format) {
			return format_time(timetype(stamp), format);
		}

		static std::string get_date(timetype);
		static std::string get_date(long);

		static std::string get_time(timetype);
		static std::string get_time(long);

		/** Returns true if all the characters in a string are valid for nicknames. */
		static bool is_valid_nick(const std::string &);

		/** Trims whitespace from the end of a string. */
		static std::string & rtrim(std::string &);
		static std::string rtrim(const std::string &);

		/** Converts mIRC colors to ANSI escapes. */
		static std::string irc2ansi(std::string);

		template <typename T, typename Iter, typename Pred>
		static std::deque<T> take_while(Iter begin, Iter end, Pred predicate) {
			std::deque<T> out {};
			while (begin != end && predicate(*begin))
				out.push_back(*begin);
			return out;
		}

		template <typename Pred>
		static std::string take_while(const std::string &str, Pred predicate) {
			std::string out {};
			for (char ch: str) {
				if (!predicate(ch))
					break;
				out.push_back(ch);
			}

			return out;
		}
	};

	template <typename T>
	struct weakptr_compare {
		bool operator()(const std::weak_ptr<T> &left, const std::weak_ptr<T> &right) const {
			std::shared_ptr<T> leftptr = left.lock(), rightptr = right.lock();

			if (!rightptr)
				return false;

			if (!leftptr)
				return true;

			return *leftptr < *rightptr;
		}
	};
}

#endif
