#ifndef PINGPONG_EVENTS_PRIVMSG_H_
#define PINGPONG_EVENTS_PRIVMSG_H_

#include "core/channel.h"
#include "core/user.h"
#include "events/event.h"

namespace pingpong {
	/**
	 * Represents a message to a user or a channel. Only one of user and chan is non-null.
	 */
	class privmsg_event: public server_event {
		public:
			enum class type {channel, user};
			std::shared_ptr<user> speaker;

			std::shared_ptr<user>    whom;
			std::shared_ptr<channel> chan;

			std::string message;

			privmsg_event(std::shared_ptr<user> speaker_, std::shared_ptr<user> whom_, const std::string &message_):
				server_event(speaker_->serv, message_), speaker(speaker_), whom(whom_), chan(nullptr) {}

			privmsg_event(std::shared_ptr<user> speaker_, std::shared_ptr<channel> chan_, const std::string &message_):
				server_event(speaker_->serv, message_), speaker(speaker_), whom(nullptr), chan(chan_) {}

			std::string get_destination() const;
	};
}

#endif
