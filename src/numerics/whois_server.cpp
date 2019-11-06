#include "pingpong/messages/numeric.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	bool numeric_message::handle_whois_server(server *serv) {
		std::vector<std::string> split = formicine::util::split(line.parameters, " ");

		if (split.size() < 4)
			throw std::runtime_error("Couldn't parse 312 numeric: too few parameters");

		const size_t colon = line.parameters.find(':');
		if (colon == std::string::npos)
			throw std::runtime_error("Couldn't parse 312 numeric: no colon");

		const std::string &nick = split[1];
		const std::string &server_name = split[2];
		const std::string &description = line.parameters.substr(colon + 1);

		serv->get_user(nick, true, true)->server_name = server_name;
		serv->server_descriptions.insert({server_name, description});
		return true;
	}
}
