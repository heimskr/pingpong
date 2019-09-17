#include "commands/privmsg.h"

#include "events/error.h"
#include "events/privmsg.h"

namespace pingpong {
	privmsg_command::privmsg_command(server *serv_, std::string destination_, std::string message_):
	command(serv_), destination(destination_), message(message_) {
		if (destination.empty())
			throw std::runtime_error("Destination is empty in privmsg_command");
	}

	privmsg_command::operator std::string() const {
		return "PRIVMSG " + destination + " :" + message;
	}

	void privmsg_command::send() {
		command::send();
		if (destination.front() == '#') {
			std::shared_ptr<channel> chan = serv->get_channel(destination);
			if (chan)
				events::dispatch<privmsg_event>(serv->get_self(), chan, message);
			else
				events::dispatch<error_event>("Can't send message: channel is null", false);
		} else {
			events::dispatch<privmsg_event>(serv->get_self(), serv->get_user(destination, true), message);
		}
	}

	bool privmsg_command::is_user() const {
		return destination.front() != '#';
	}

	bool privmsg_command::is_channel() const {
		return destination.front() == '#';
	}

	std::shared_ptr<user> privmsg_command::get_user() const {
		return is_user()? serv->get_user(destination) : nullptr;
	}

	std::shared_ptr<channel> privmsg_command::get_channel() const {
		return is_channel()? serv->get_channel(destination) : nullptr;
	}
}
