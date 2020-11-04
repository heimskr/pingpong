#ifndef PINGPONG_CORE_UTIL_H_
#define PINGPONG_CORE_UTIL_H_

#include <array>
#include <deque>
#include <memory>
#include <chrono>
#include <string>

namespace PingPong {
	/**
	 * Contains miscellaneous utility functions for pingpong.
	 */
	struct Util {

		static constexpr const char *nickChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
			"-_[]{}\\`|";
		static constexpr const char *flagChars = "+-ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		static std::array<std::string, 100> ircColors;

		using TimeType = std::chrono::microseconds;

		static TimeType now();
		static std::chrono::system_clock::duration system_now();

		static constexpr long precision = 1'000'000;

		static long timestamp();
		static long seconds();
		static long millistamp();
		static long microstamp();
		static long nanostamp();

		static TimeType fromSeconds(long);

		template <size_t Size = 64>
		static std::string formatTime(TimeType when, const char *format) {
			std::chrono::system_clock::time_point tpoint {when};
			std::time_t time = std::chrono::system_clock::to_time_t(tpoint);
			char str[Size];
			std::strftime(str, sizeof(str), format, std::localtime(&time));
			return str;
		}

		template <size_t Size = 64>
		static std::string formatTime(long stamp, const char *format) {
			return formatTime(TimeType(stamp), format);
		}

		static std::string getDate(TimeType);
		static std::string getDate(long);

		static std::string getTime(TimeType);
		static std::string getTime(long);

		/** Returns true if all the characters in a string are valid for nicknames. */
		static bool isValidNick(const std::string &);

		/** Trims whitespace from the end of a string. */
		static std::string & rtrim(std::string &);
		static std::string rtrim(const std::string &);

		/** Converts mIRC colors to ANSI escapes. */
		static std::string irc2ansi(std::string);

		template <typename T, typename Iter, typename Pred>
		static std::deque<T> takeWhile(Iter begin, Iter end, Pred predicate) {
			std::deque<T> out {};
			while (begin != end && predicate(*begin))
				out.push_back(*begin);
			return out;
		}

		template <typename Pred>
		static std::string takeWhile(const std::string &str, Pred predicate) {
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
