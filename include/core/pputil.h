#ifndef PINGPONG_CORE_UTIL_H_
#define PINGPONG_CORE_UTIL_H_

#include <memory>
#include <chrono>
#include <string>

namespace pingpong {
	/**
	 * Contains miscellaneous utility functions for pingpong.
	 */
	class util {
		private:
			static std::chrono::system_clock::duration now();
			static constexpr const char *nick_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
				"-_[]{}\\`|";

		public:
			static long timestamp();
			static long millistamp();
			static long microstamp();
			static long nanostamp();

			/** Returns true if all the characters in a string are valid for nicknames. */
			static bool is_valid_nick(const std::string &);
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
