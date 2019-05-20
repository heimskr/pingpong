#ifndef CORE_DEFS_H_
#define CORE_DEFS_H_

#include <memory>

namespace pingpong {
	class channel;
	class server;
	class irc;

	using server_ptr = pingpong::server *;
}

#endif
