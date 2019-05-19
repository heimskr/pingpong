#include <string>

#include "channel.h"

namespace pingpong {
	bool channel::is_user() {
		return name.at(0) == '#';
	}
}
