#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <ostream>
#include <string>

#include "defs.h"
#include "server.h"

namespace pingpong {
	class channel {
		public:
			std::string name;
			server_ptr serv;

			channel(std::string, server_ptr);
			channel(std::string);
			bool is_user() const;

			operator std::string() const { return name; }
			bool has_server() const { return serv != nullptr; }

			friend std::ostream & operator<<(std::ostream &os, const channel &chan);
	};
}

#endif
