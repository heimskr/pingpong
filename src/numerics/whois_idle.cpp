#include "pingpong/messages/numeric.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	bool numeric_message::handle_whois_idle(server *serv) {
		const std::string &nick = formicine::util::nth_word(line.parameters, 1);
		const std::string &idle_time_str = formicine::util::nth_word(line.parameters, 2);
		const std::string &word3 = formicine::util::nth_word(line.parameters, 3);

		std::shared_ptr<user> user_ptr = serv->get_user(nick, true, true);

		if (word3.front() != ':' && !formicine::util::parse_long(word3, user_ptr->signon_time))
			throw std::runtime_error("Couldn't parse signon time as a number in numeric 317");
		
		long idle_time;
		if (!formicine::util::parse_long(idle_time_str, idle_time))
			throw std::runtime_error("Couldn't parse idle-since as a number in numeric 317");
		
		user_ptr->idle_since = util::seconds() - idle_time;
		return true;
	}
}
