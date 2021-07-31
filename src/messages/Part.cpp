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

		// PARTs without a message look like "PART #foo". PARTS with a message look like "PART #foo :bar".
		// Due to the lack of a colon, SourcedMessage's constructor interprets the channel of a messageless PART as its
		// content, even though that's not the case. I could implement a workaround to prevent this behavior if virtual
		// methods were callable from constructors, but because they're not, I have to read the Line here.
		if (line.parameters.find(':') == std::string::npos)
			Events::dispatch<PartEvent>(who, chan, "");
		else
			Events::dispatch<PartEvent>(who, chan, content);

		return true;
	}
}
