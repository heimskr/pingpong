#include "pingpong/commands/notice.h"

#include "pingpong/events/error.h"
#include "pingpong/events/notice.h"

namespace pingpong {
	notice_command::notice_command(server *serv_, std::string where_, std::string message_, bool hidden_):
	command(serv_), local(where_), message(message_), hidden(hidden_) {
		if (where.empty())
			throw std::runtime_error("Destination is empty in notice_command");
	}

	notice_command::operator std::string() const {
		return "NOTICE " + where + " :" + message;
	}

	bool notice_command::send() {
		if (command::send()) {
			if (is_channel()) {
				std::shared_ptr<channel> chan = get_channel(serv);
				if (chan) {
					if (!hidden)
						events::dispatch<notice_event>(serv->get_self(), chan, message, hidden);
				} else {
					events::dispatch<error_event>("Can't send notice: channel is null", false);
				}
			} else if (!hidden) {
				events::dispatch<notice_event>(serv->get_self(), get_user(serv, false), message, hidden);
			}

			return true;
		}

		return false;
	}
}
