#include "pingpong/commands/Kick.h"
#include "pingpong/core/Channel.h"

namespace PingPong {
	KickCommand::KickCommand(Server *server_, std::shared_ptr<Channel> channel_, const std::string &whom_,
	const std::string &reason_):
		KickCommand(server_, channel_->name, whom_, reason_) {}
				
	KickCommand::operator std::string() const {
		return "KICK " + channel + " " + whom + " :" + reason;
	}
}
