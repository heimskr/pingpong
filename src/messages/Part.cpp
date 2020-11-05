#include <string>

#include "pingpong/events/Part.h"
#include "pingpong/events/NamesUpdated.h"

#include "pingpong/messages/Part.h"

namespace PingPong {
	PartMessage::operator std::string() const {
		return who->name + " left " + where + (content.empty()? "" : " [" + content + "]");
	}

	bool PartMessage::operator()(Server *server) {
		if (where.empty() || where.front() != '#')
			throw std::runtime_error("Invalid channel for PartMessage");
		
		std::shared_ptr<Channel> chan = server->getChannel(where, true);

		if (who->isSelf()) {
			server->removeChannel(where);
		} else {
			chan->removeUser(who);
			Events::dispatch<NamesUpdatedEvent>(chan);
		}

		Events::dispatch<PartEvent>(who, chan, content);
		return true;
	}
}
