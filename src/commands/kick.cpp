#include "pingpong/commands/kick.h"
#include "pingpong/core/channel.h"

namespace pingpong {
	kick_command::kick_command(server *serv_, std::shared_ptr<channel> chan_, const std::string &whom_,
	const std::string &reason_):
		kick_command(serv_, chan_->name, whom_, reason_) {}
				
	kick_command::operator std::string() const {
		return "KICK " + chan + " " + whom + " :" + reason;
	}
}
