#ifndef COMMANDS_COMMAND_H_
#define COMMANDS_COMMAND_H_

#include <memory>

#include "core/pputil.h"
#include "core/server.h"

namespace pingpong {
	class command {
		protected:
			server_ptr serv;

		public:
			long sent_time = util::timestamp();

			command(server_ptr serv_): serv(serv_) {}
			virtual operator std::string() const = 0;

			virtual void send();

			virtual bool is_silent() const { return false; }
	};
}

#endif
