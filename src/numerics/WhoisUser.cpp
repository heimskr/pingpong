#include "pingpong/messages/Numeric.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	bool NumericMessage::handleWhoisUser(Server *server) {
		const size_t colon = line.parameters.find(':');
		if (colon == std::string::npos)
			throw std::runtime_error("Couldn't parse 311 numeric: no colon found");

		try {
			std::vector<std::string> split = formicine::util::split(line.parameters, " ");
			const std::string &nickname = split[1];
			const std::string &username = split[2];
			const std::string &hostname = split[3];

			const std::string  realname = line.parameters.substr(colon + 1);

			std::shared_ptr<User> who = server->getUser(nickname, true, true);
			who->info.user = username;
			who->info.host = hostname;
			DBG("setting realname to [" << realname << "]");
			who->realname  = realname;
			DBG("who[" << who.get() << "]");
			DBG("who->realname[" << who->realname << "]");
		} catch (const std::exception &err) {
			DBG("Couldn't parse whois_user: " << err.what());
			throw std::runtime_error("Couldn't parse 311 numeric");
		}

		return true;
	}
}
