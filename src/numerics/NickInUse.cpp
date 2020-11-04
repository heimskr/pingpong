#include "pingpong/events/NickInUse.h"
#include "pingpong/messages/Numeric.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	bool NumericMessage::handleNickInUse(Server *server) {
		Events::dispatch<NickInUseEvent>(formicine::util::nth_word(line.parameters, 1), server);
		return true;
	}
}
