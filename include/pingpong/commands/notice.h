#ifndef PINGPONG_COMMANDS_NOTICE_H_
#define PINGPONG_COMMANDS_NOTICE_H_

#include <string>

#include "pingpong/core/Local.h"
#include "Command.h"

namespace PingPong {
	class NoticeCommand: public Command, public Local {
		public:
			std::string message;
			
			/** Whether to set the hidden field of the generated notice_event to true. */
			bool hidden;

			NoticeCommand(Server *server_, std::string where_, std::string message_, bool hidden_ = false);

			NoticeCommand(std::shared_ptr<Channel> channel, std::string message_, bool hidden_ = false):
				NoticeCommand(channel->server, channel->name, message_, hidden_) {}

			NoticeCommand(std::shared_ptr<User> user, std::string message_, bool hidden_ = false):
				NoticeCommand(user->server, user->name, message_, hidden_) {}

			operator std::string() const override;
			virtual bool send() override;
	};
}

#endif
