#include "pingpong/messages/cap.h"
#include "pingpong/core/features.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	cap_message::operator std::string() const {
		return line.original;
	}

	bool cap_message::operator()(server *serv) {
		const size_t colon = line.parameters.find(':');
		const std::string sub = formicine::util::nth_word(line.parameters, 1);

		if (colon == std::string::npos && sub == "ACK")
			throw std::invalid_argument("No parameter list found in CAP command");
		
		if (sub == "ACK") {
			ack(serv, formicine::util::split(line.parameters.substr(colon + 1), " ", true));
		}

		return true;
	}

	void cap_message::ack(server *serv, const std::vector<std::string> &caps) {
		for (const std::string &cap: caps) {
			if (features::types.count(cap) == 0) {
				DBG("Unknown feature: \"" << cap << "\"");
			} else {
				DBG("Adding feature: \"" << cap << "\"");
				serv->add_feature(features::types.at(cap));
			}
		}
	}
}
