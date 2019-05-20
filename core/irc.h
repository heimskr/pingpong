#ifndef CORE_IRC_H_
#define CORE_IRC_H_

#include <mutex>
#include <vector>

#include "lib/ansi.h"
#include "defs.h"

namespace pingpong {
	class irc {
		private:
			std::vector<server_ptr> servers;
			std::mutex console_mux = std::mutex();
			ansi::ansistream dbg;

		public:
			static constexpr int default_port = 6667;
			std::string username, realname;

			irc(std::string user, std::string real): username(user), realname(real) {}
			irc(): irc("pingpong", "PingPong IRC") {}
			
			std::unique_lock<std::mutex> lock_console() { return std::unique_lock(console_mux); }
		
			template <typename T>
			ansi::ansistream & operator<<(const T &value) { return dbg << value; }
			ansi::ansistream & dbgout() { return dbg << " >> "_d; }
			ansi::ansistream & dbgin()  { return dbg << "<<  "_d; }
	};
}

#endif
