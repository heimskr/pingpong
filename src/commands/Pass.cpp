#include "pingpong/commands/Pass.h"

namespace PingPong {
	PassCommand::operator std::string() const {
		return "PASS " + password;
	}
}
