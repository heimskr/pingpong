#include "pingpong/events/Privmsg.h"
#include "pingpong/events/VersionRequested.h"
#include "pingpong/messages/Privmsg.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	PrivmsgMessage::operator std::string() const {
		return "[" + where + "] <" + who->name + "> " + content;
	}

	bool PrivmsgMessage::operator()(Server *) {
		if (formicine::util::upper(content) == "\x01VERSION\x01") {
			Events::dispatch<VersionRequestedEvent>(who, where, content);
		} else {
			Events::dispatch<PrivmsgEvent>(who, where, content, line);
		}

		return true;
	}
}