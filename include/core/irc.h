#ifndef CORE_IRC_H_
#define CORE_IRC_H_

#include <memory>
#include <mutex>
#include <set>

#include "core/defs.h"
#include "lib/ansi.h"

namespace pingpong {
	class irc {
		private:
			std::mutex console_mux = std::mutex();

		public:
			static constexpr int default_port = 6667;
			static constexpr const char *default_nick = "pingpong";

			std::string username, realname;
			static ansi::ansistream dbg;

			std::set<server_ptr> servers;
			server_ptr active_server = nullptr;

			irc(std::string user, std::string real): username(user), realname(real) {}
			irc(): irc("pingpong", "PingPong IRC") {}
			
			std::unique_lock<std::mutex> lock_console() { return std::unique_lock(console_mux); }
			void init();
			void init_messages();

			irc & operator+=(const server_ptr &ptr);
		
			template <typename T>
			ansi::ansistream & operator<<(const T &value) { return dbg << value; }
			ansi::ansistream & dbgout()  { return dbg << " >> "_d; }
			ansi::ansistream & dbgin()   { return dbg << "<<  "_d; }
	};
}

#endif
