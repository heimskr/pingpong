#include "pingpong/commands/cap.h"
#include "pingpong/core/server.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	cap_command::operator std::string() const {
		if (type == action::end) {
			if (serv->get_status() == server::stage::capneg)
				serv->set_status(server::stage::setuser);
			return "CAP END";
		}

		if (type == action::ls)
			return "CAP LS 302";

		if (type == action::req) {
			std::vector<const char *> request_chars;
			std::transform(to_request.begin(), to_request.end(), std::back_inserter(request_chars),
				[&](features::type feature) { return features::names[feature]; });

			serv->sent_cap_req(to_request.size());
			return "CAP REQ :" + formicine::util::join(request_chars, " ");
		}

		throw std::invalid_argument("Invalid action type: " + std::to_string(static_cast<int>(type)));
	}
}
