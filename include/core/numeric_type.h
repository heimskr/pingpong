#ifndef PINGPONG_CORE_NUMERIC_TYPE_H_
#define PINGPONG_CORE_NUMERIC_TYPE_H_

namespace pingpong {
	enum class numeric_type: int {
		channel_mode_is	= 324,
		channel_topic	= 332,
		topic_modified	= 333,
		names_reply		= 353,
		motd			= 372,
		motd_start		= 375,
		motd_end		= 376,
		no_such_nick	= 401
	};
}

#endif
