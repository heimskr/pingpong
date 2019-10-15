#include "pingpong/messages/cap.h"
#include "pingpong/commands/cap.h"
#include "pingpong/commands/user.h"
#include "pingpong/core/features.h"
#include "pingpong/core/server.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	cap_message::operator std::string() const {
		return line.original;
	}

	bool cap_message::operator()(server *serv) {
		const size_t colon = line.parameters.find(':');
		const std::string sub = formicine::util::nth_word(line.parameters, 1);

		if (colon == std::string::npos && (sub == "ACK" || sub == "NAK"))
			throw std::invalid_argument("No parameter list found in CAP command");

		if (sub == "LS") {
			serv->support_features(line.parameters.substr(colon + 1));
			if (serv->status == server::stage::capneg) {
				std::unordered_set<features::type> common_subset {};
				for (features::type feature: serv->get_supported_features()) {
					if (features::implemented.count(feature) != 0)
						common_subset.insert(feature);
				}

				cap_command(serv, common_subset, cap_command::action::req).send();
			}
		} else if (sub == "ACK") {
			std::vector<std::string> caps = formicine::util::split(line.parameters.substr(colon + 1), " ", true);
			ack(serv, caps);
			serv->cap_answered(caps.size());
		} else if (sub == "NAK") {
			serv->cap_answered(1 + std::count(line.parameters.begin() + colon + 1, line.parameters.end(), ' '));
			DBG("Denied features: " << line.parameters.substr(colon + 1));
		}

		return true;
	}

	void cap_message::ack(server *serv, const std::vector<std::string> &caps) {
		for (const std::string &cap: caps) {
			if (features::types.count(cap) == 0) {
				DBG("Unknown feature: \"" << cap << "\"");
			} else {
				serv->add_feature(features::types[cap]);
			}
		}
	}
}
