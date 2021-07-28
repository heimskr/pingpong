#include <cstdlib>
#include <string>

#include "pingpong/commands/Command.h"
#include "pingpong/events/Command.h"
	
namespace PingPong {
	std::function<bool(Command &)> Command::beforeSend = [](Command &) { return true; };

	Command & Command::setLabel(const std::string &new_label) {
		label = new_label;
		return *this;
	}

	Command & Command::setLabel() {
		label.clear();
		for (int i = 0; i < 10; ++i) {
			int n = rand() % 62;
			if (n < 10)
				label += std::to_string(n);
			else if (n < 36)
				label += 'a' - 10 + n;
			else
				label += 'A' - 36 + n;
		}
		return *this;
	}

	bool Command::send() {
		if (beforeSend(*this)) {
			Events::dispatch<CommandEvent>(server, this);
			if (label.empty() || !server->featureEnabled(Features::Type::LabeledResponse))
				server->quote(std::string(*this));
			else
				server->quote("@label=" + label + " " + std::string(*this));
			return true;
		}

		return false;
	}
}
