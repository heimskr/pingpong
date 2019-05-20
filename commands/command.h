#ifndef COMMANDS_COMMAND_H_
#define COMMANDS_COMMAND_H_

#include <memory>

#include "core/server.h"

namespace pingpong {
	class command {
		protected:
			server_ptr serv;

		public:
			command(server_ptr serv_): serv(serv_) {}
			virtual operator std::string() const = 0;

			virtual void send() {
				serv->quote(std::string(*this));
			}
	};
}

#endif
