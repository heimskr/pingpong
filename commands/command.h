#ifndef COMMANDS_COMMAND_H_
#define COMMANDS_COMMAND_H_

#include <memory>

#include "core/server.h"

namespace pingpong {
	class command {
		protected:
			serv_ptr serv;

		public:
			command(serv_ptr serv_): serv(serv_) {}
			virtual operator std::string() const = 0;

			void send() {
				serv->quote(std::string(*this));
			}
	};
}

#endif
