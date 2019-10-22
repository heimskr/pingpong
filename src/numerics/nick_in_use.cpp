#include "pingpong/events/nick_in_use.h"
#include "pingpong/messages/numeric.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	bool numeric_message::handle_nick_in_use(server *serv) {
		events::dispatch<nick_in_use_event>(formicine::util::nth_word(line.parameters, 1), serv);
		return true;
	}
}
