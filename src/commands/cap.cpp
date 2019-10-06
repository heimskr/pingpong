#include "pingpong/commands/cap.h"
#include "lib/formicine/futil.h"

namespace pingpong {
	cap_command::operator std::string() const {
		if (to_request.empty())
			return "CAP LS 302";

		std::vector<const char *> request_chars;
		std::transform(to_request.begin(), to_request.end(), std::back_inserter(request_chars),
			[&](features::type feature) { return features::names[feature]; });

		return "CAP REQ :" + formicine::util::join(request_chars, " ");
	}
}
