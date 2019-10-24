#ifndef PINGPONG_COMMANDS_NOTICE_H_
#define PINGPONG_COMMANDS_NOTICE_H_

#include <string>

#include "pingpong/core/local.h"
#include "command.h"

namespace pingpong {
	class notice_command: public command, public local {
		public:
			std::string message;
			
			/** Whether to set the hidden field of the generated notice_event to true. */
			bool hidden;

			notice_command(server *serv_, std::string where_, std::string message_, bool hidden_ = false);

			notice_command(std::shared_ptr<channel> chan, std::string message_, bool hidden_ = false):
				notice_command(chan->serv, chan->name, message_, hidden_) {}

			notice_command(std::shared_ptr<user> user, std::string message_, bool hidden_ = false):
				notice_command(user->serv, user->name, message_, hidden_) {}

			operator std::string() const override;
			virtual bool send() override;
	};
}

#endif
