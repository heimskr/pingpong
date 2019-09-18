#ifndef COMMANDS_COMMAND_H_
#define COMMANDS_COMMAND_H_

#include "core/pputil.h"
#include "core/server.h"

namespace pingpong {
	class command {
		public:
			server *serv;
			long sent_time = util::timestamp();

			command(server *serv_): serv(serv_) {}
			virtual operator std::string() const = 0;

			virtual void send();

			virtual bool is_silent() const { return false; }
	};
}

#endif
