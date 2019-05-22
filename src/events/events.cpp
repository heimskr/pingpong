#include <functional>
#include <map>
#include <string>

#include "events/events.h"

namespace pingpong {
	std::multimap<std::string, listener_fn> events::listeners;
}
