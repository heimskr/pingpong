#include "core/pputil.h"

namespace pingpong {
	std::chrono::system_clock::duration util::now() {
		return std::chrono::system_clock::now().time_since_epoch();
	}

	long util::timestamp() {
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
}
