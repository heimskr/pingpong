#include "pingpong/messages/cap.h"
#include "pingpong/commands/cap.h"
#include "pingpong/commands/user.h"
#include "pingpong/core/features.h"

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

		if (sub == "ACK") {
			ack(serv, formicine::util::split(line.parameters.substr(colon + 1), " ", true));

			if (serv->get_status() == server::stage::capneg) {
				cap_command(serv, cap_command::action::end).send();
				user_command(serv, serv->get_parent()->username, serv->get_parent()->realname).send();
			}
		} else if (sub == "NAK") {
			DBG("Denied features: " << line.parameters.substr(colon + 1));
		}

		return true;
	}

	void cap_message::ack(server *serv, const std::vector<std::string> &caps) {
		for (const std::string &cap: caps) {
			if (features::types.count(cap) == 0) {
				DBG("Unknown feature: \"" << cap << "\"");
			} else {
				serv->add_feature(features::types.at(cap));
			}
		}
	}
}
