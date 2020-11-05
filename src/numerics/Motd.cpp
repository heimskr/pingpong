#include "pingpong/core/ParseError.h"
#include "pingpong/messages/Numeric.h"
#include "pingpong/events/Motd.h"

namespace PingPong {
	bool NumericMessage::handleMotd(Server *server) {
		const size_t colon = line.parameters.find(':');
		if (colon == std::string::npos)
			throw ParseError(line.parameters, "No colon in MOTD line");
		Events::dispatch<MotdEvent>(server, line.parameters.substr(colon + 1));
		return true;
	}
}
