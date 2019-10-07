#include <iostream>
#include <string>

#include "pingpong/core/server.h"
#include "pingpong/events/join.h"
#include "pingpong/events/names_updated.h"
#include "pingpong/messages/join.h"

#include "lib/formicine/futil.h"

namespace pingpong {
	join_message::join_message(const pingpong::line &line_): message(line_), serv(line_.serv), who(line_.source.nick) {
		const std::string &params = line_.parameters;
		DBG("join_message. [" << params << "]");
		if (params.empty())
			throw std::invalid_argument("Parameters for a JOIN message cannot be empty");

		// The parameters for a traditional JOIN message look like ":#channel".
		// When extended-join is enabled, they look like "#channel accountname :real name" (accountname can be "*").
		const char front = params.front();
		if (front == ':') { // traditional
			chan = params.substr(1);
		} else if (front == '#' || front == '&') { // extended-join
			if (std::count(params.begin(), params.end(), ' ') < 2)
				throw std::invalid_argument("Parameters for a extended JOIN message must contain at least three words");

			chan = formicine::util::nth_word(params, 0, false);
			accountname = formicine::util::nth_word(params, 1, false);

			const size_t second_space = formicine::util::nth_index(params, ' ', 1);
			if (params[second_space + 1] != ':') {
				throw std::invalid_argument("Expected a colon at the start of the realname parameter in an extended "
				                            "JOIN message");
			}

			realname = params.substr(second_space + 2);
		}
	}

	join_message::operator std::string() const {
		return who + " joined " + chan;
	}

	bool join_message::operator()(server *serv) {
		if (chan.empty() || chan.front() != '#')
			throw std::runtime_error("Invalid channel for join_message");

		std::shared_ptr<user> userptr = serv->get_user(who, true);
		std::shared_ptr<channel> chanptr = serv->get_channel(chan, true);

		if (accountname == "*") {
			// If the accountname in an extended JOIN message is '*', the user isn't identified.
			userptr->account_name.clear();
		} else if (!accountname.empty()) {
			userptr->account_name = accountname;
		}

		if (!chanptr->has_user(userptr)) {
			chanptr->add_user(userptr);
			events::dispatch<names_updated_event>(chanptr);
		}

		events::dispatch<join_event>(userptr, chanptr);
		return true;
	}
}