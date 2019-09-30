#ifndef PINGPONG_COMMANDS_MODE_H_
#define PINGPONG_COMMANDS_MODE_H_

#include "pingpong/core/local.h"
#include "command.h"

namespace pingpong {
	class mode_command: public command, public local {
		public:
			std::string flags, extra;

			mode_command(const std::string &where_, server *serv_, const std::string &flags_ = "",
			const std::string &extra_ = ""):
				command(serv_), local(where_), flags(flags_), extra(extra_) {}

			mode_command(std::shared_ptr<user> who, const std::string &flags_ = "", const std::string &extra_ = ""):
				mode_command(who->name, who->serv, flags_, extra_) {}

			mode_command(std::shared_ptr<channel> chan, const std::string &flags_ = "", const std::string &extra_ = ""):
				mode_command(chan->name, chan->serv, flags_, extra_) {}

			operator std::string() const override;
	};
}

#endif
