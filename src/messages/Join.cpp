#include <iostream>
#include <string>

#include "pingpong/core/Server.h"
#include "pingpong/events/Join.h"
#include "pingpong/events/NamesUpdated.h"
#include "pingpong/messages/Join.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	JoinMessage::JoinMessage(const PingPong::Line &line_):
	Message(line_), server(line_.server), who(line_.source.nick) {
		const std::string &params = line_.parameters;
		if (params.empty())
			throw std::invalid_argument("Parameters for a JOIN message cannot be empty");

		// The parameters for a traditional JOIN message look like ":#channel".
		// When extended-join is enabled, they look like "#channel accountname :real name" (accountname can be "*").
		const char front = params.front();
		if (front == ':') { // traditional
			chan = params.substr(1);
		} else if (front == '#' || front == '&') { // extended-join
			if (std::count(params.begin(), params.end(), ' ') < 2)
				throw std::invalid_argument("Parameters for an extended JOIN message must contain at least three "
					"words");

			chan = formicine::util::nth_word(params, 0, false);
			accountName = formicine::util::nth_word(params, 1, false);

			const size_t second_space = formicine::util::nth_index(params, ' ', 2);
			if (params[second_space + 1] != ':') {
				throw std::invalid_argument("Expected a colon at the start of the realname parameter in an extended "
				                            "JOIN message");
			}

			realname = params.substr(second_space + 2);
		}
	}

	JoinMessage::operator std::string() const {
		return who + " joined " + chan;
	}

	bool JoinMessage::operator()(Server *server) {
		if (chan.empty() || chan.front() != '#')
			throw std::runtime_error("Invalid channel for JoinMessage");

		std::shared_ptr<User> userptr = server->getUser(who, true, true);
		std::shared_ptr<Channel> chanptr = server->getChannel(chan, true);

		if (accountName == "*") {
			// If the accountname in an extended JOIN message is '*', the user isn't identified.
			userptr->accountName.clear();
		} else if (!accountName.empty()) {
			userptr->accountName = accountName;
		}

		if (!chanptr->hasUser(userptr)) {
			chanptr->addUser(userptr);
			Events::dispatch<NamesUpdatedEvent>(chanptr);
		}

		chanptr->announced = true;
		return true;
	}
}
