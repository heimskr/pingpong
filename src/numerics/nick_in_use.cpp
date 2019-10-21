#include "pingpong/events/nick_in_use.h"
#include "pingpong/messages/numeric.h"

namespace pingpong {
	bool numeric_message::handle_nick_in_use(server *serv) {
		events::dispatch<nick_in_use_event>(line.parameters.substr(line.parameters.find(' ')), serv);
		return true;
	}
}
