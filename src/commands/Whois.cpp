#include "pingpong/commands/Whois.h"
#include "pingpong/core/User.h"

namespace PingPong {
	WhoisCommand::WhoisCommand(Server *server_, std::shared_ptr<User> whom_):
		WhoisCommand(server_, whom_->name) {}

	WhoisCommand::operator std::string() const {
		return "WHOIS " + whom;
	}
}
