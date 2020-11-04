#include "pingpong/messages/Numeric.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	bool NumericMessage::handleWhoisIdle(Server *server) {
		const std::string &nick = formicine::util::nth_word(line.parameters, 1);
		const std::string &idle_time_str = formicine::util::nth_word(line.parameters, 2);
		const std::string &word3 = formicine::util::nth_word(line.parameters, 3);

		std::shared_ptr<User> user_ptr = server->getUser(nick, true, true);

		if (word3.front() != ':' && !formicine::util::parse_long(word3, user_ptr->signonTime))
			throw std::runtime_error("Couldn't parse signon time as a number in numeric 317");
		
		long idle_time;
		if (!formicine::util::parse_long(idle_time_str, idle_time))
			throw std::runtime_error("Couldn't parse idle-since as a number in numeric 317");
		
		user_ptr->idleSince = Util::seconds() - idle_time;
		return true;
	}
}
