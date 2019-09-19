#ifndef PINGPONG_CORE_NUMERIC_TYPE_H_
#define PINGPONG_CORE_NUMERIC_TYPE_H_

namespace pingpong {
	enum class numeric_type: int {
		channel_mode_is = 324,
		names_reply     = 353,
		no_such_nick    = 401
	};
}

#endif
