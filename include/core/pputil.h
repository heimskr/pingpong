#ifndef PINGPONG_CORE_UTIL_H_
#define PINGPONG_CORE_UTIL_H_

#include <chrono>

namespace pingpong {
	/**
	 * Contains miscellaneous utility functions for pingpong.
	 */
	class util {
		private:
			static std::chrono::system_clock::duration now();

		public:
			static long timestamp();
			static long millistamp();
			static long microstamp();
			static long nanostamp();
	};
}

#endif
