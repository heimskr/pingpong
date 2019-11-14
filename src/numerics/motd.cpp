#include "pingpong/messages/numeric.h"
#include "pingpong/events/motd.h"

namespace pingpong {
	bool numeric_message::handle_motd(server *serv) {
		const size_t colon = line.parameters.find(':');
		if (colon == std::string::npos)
			throw parse_error(line.parameters, "No colon in MOTD line");
		events::dispatch<motd_event>(serv, line.parameters.substr(colon + 1));
		return true;
	}
}
