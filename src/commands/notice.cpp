#include "pingpong/commands/Notice.h"

#include "pingpong/events/Error.h"
#include "pingpong/events/Notice.h"

namespace PingPong {
	NoticeCommand::NoticeCommand(Server *server_, std::string where_, std::string message_, bool hidden_):
	Command(server_), Local(where_), message(message_), hidden(hidden_) {
		if (where.empty())
			throw std::runtime_error("Destination is empty in NoticeCommand");
	}

	NoticeCommand::operator std::string() const {
		return "NOTICE " + where + " :" + message;
	}

	bool NoticeCommand::send() {
		if (Command::send()) {
			if (isChannel()) {
				std::shared_ptr<Channel> channel = getChannel(server);
				if (channel) {
					if (!hidden)
						Events::dispatch<NoticeEvent>(server->getSelf(), channel, message, hidden);
				} else {
					Events::dispatch<ErrorEvent>("Can't send notice: channel is null", false);
				}
			} else if (!hidden) {
				Events::dispatch<NoticeEvent>(server->getSelf(), getUser(server, false), message, hidden);
			}

			return true;
		}

		return false;
	}
}
