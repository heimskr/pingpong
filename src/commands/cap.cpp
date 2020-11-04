#include "pingpong/commands/Cap.h"
#include "pingpong/core/Server.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	CapCommand::operator std::string() const {
		if (type == Action::End) {
			if (server->getStatus() == Server::Stage::CapNeg)
				server->setStatus(Server::Stage::SetUser);
			return "CAP END";
		}

		if (type == Action::Ls)
			return "CAP LS 302";

		if (type == Action::Req) {
			std::vector<const char *> request_chars;
			std::transform(to_request.begin(), to_request.end(), std::back_inserter(request_chars),
				[&](Features::Type feature) { return Features::names[feature]; });

			server->sentCapReq(to_request.size());
			return "CAP REQ :" + formicine::util::join(request_chars, " ");
		}

		throw std::invalid_argument("Invalid action type: " + std::to_string(static_cast<int>(type)));
	}
}
