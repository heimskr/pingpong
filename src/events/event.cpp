#include <map>
#include <string>

#include "events/event.h"

namespace pingpong {
	std::multimap<std::string, listener_fn> events::listeners;
}
