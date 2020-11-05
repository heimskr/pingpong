#include "pingpong/messages/Cap.h"
#include "pingpong/commands/Cap.h"
#include "pingpong/commands/User.h"
#include "pingpong/core/Features.h"
#include "pingpong/core/Server.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	CapMessage::operator std::string() const {
		return line.original;
	}

	bool CapMessage::operator()(Server *server) {
		const size_t colon = line.parameters.find(':');
		const std::string sub = formicine::util::nth_word(line.parameters, 1);

		if (colon == std::string::npos && (sub == "ACK" || sub == "NAK"))
			throw std::invalid_argument("No parameter list found in CAP command");

		if (sub == "LS") {
			server->supportFeatures(line.parameters.substr(colon + 1));
			if (server->status == Server::Stage::CapNeg) {
				std::unordered_set<Features::Type> common_subset {};
				for (Features::Type feature: server->getSupportedFeatures()) {
					if (Features::implemented.count(feature) != 0)
						common_subset.insert(feature);
				}

				CapCommand(server, common_subset, CapCommand::Action::Req).send();
			}
		} else if (sub == "ACK") {
			std::vector<std::string> caps = formicine::util::split(line.parameters.substr(colon + 1), " ", true);
			ack(server, caps);
			server->capAnswered(caps.size());
		} else if (sub == "NAK") {
			server->capAnswered(1 + std::count(line.parameters.begin() + colon + 1, line.parameters.end(), ' '));
			DBG("Denied features: " << line.parameters.substr(colon + 1));
		}

		return true;
	}

	void CapMessage::ack(Server *server, const std::vector<std::string> &caps) {
		for (const std::string &cap: caps) {
			if (Features::types.count(cap) == 0) {
				DBG("Unknown feature: \"" << cap << "\"");
			} else {
				server->addFeature(Features::types[cap]);
			}
		}
	}
}
