#include "pingpong/commands/whois.h"
#include "pingpong/core/user.h"

namespace pingpong {
	whois_command::whois_command(server *serv_, std::shared_ptr<user> whom_):
		whois_command(serv_, whom_->name) {}

	whois_command::operator std::string() const {
		return "WHOIS " + whom;
	}
}
