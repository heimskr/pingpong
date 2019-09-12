#include "core/pputil.h"

namespace pingpong {
	std::chrono::system_clock::duration util::now() const {
		return std::chrono::system_clock::now().time_since_epoch();
	}

	long util::timestamp() const {
		return std::chrono::duration_cast<std::chrono::seconds>(now()).count();
	}

	long util::millistamp() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(now()).count();
	}

	long util::microstamp() const {
		return std::chrono::duration_cast<std::chrono::microseconds>(now()).count();
	}

	long util::nanostamp() const {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(now()).count();
	}
}
