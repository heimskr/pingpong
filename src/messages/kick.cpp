#include "pingpong/events/Kick.h"
#include "pingpong/events/NamesUpdated.h"
#include "pingpong/messages/Kick.h"

namespace PingPong {
	KickMessage::operator std::string() const {
		return who->name + " kicked " + whom->name + " from " + channel->name + " [" + content + "]";
	}

	bool KickMessage::operator()(Server *) {
		channel->removeUser(whom);
		Events::dispatch<NamesUpdatedEvent>(channel);
		Events::dispatch<KickEvent>(who, whom, channel, content);
		return true;
	}
}
