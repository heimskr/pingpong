#include "pingpong/messages/Numeric.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	bool NumericMessage::handleWhoisServer(Server *server) {
		std::vector<std::string> split = formicine::util::split(line.parameters, " ");

		if (split.size() < 4)
			throw std::runtime_error("Couldn't parse 312 numeric: too few parameters");

		const size_t colon = line.parameters.find(':');
		if (colon == std::string::npos)
			throw std::runtime_error("Couldn't parse 312 numeric: no colon");

		const std::string &nick = split[1];
		const std::string &server_name = split[2];
		const std::string &description = line.parameters.substr(colon + 1);

		server->getUser(nick, true, true)->serverName = server_name;
		server->serverDescriptions.insert({server_name, description});
		return true;
	}
}
