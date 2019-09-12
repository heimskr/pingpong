#ifndef PINGPONG_CORE_UTIL_H_
#define PINGPONG_CORE_UTIL_H_

#include <chrono>

namespace pingpong {
	/**
	 * Contains miscellaneous utility functions for pingpong.
	 */
	class util {
		private:
			std::chrono::system_clock::duration now() const;

		public:
			long timestamp() const;
			long millistamp() const;
			long microstamp() const;
			long nanostamp() const;
	};
}

#endif
