#ifndef PINGPONG_CORE_IRC_H_
#define PINGPONG_CORE_IRC_H_

#include <list>
#include <map>
#include <memory>
#include <mutex>

#include "core/ppdefs.h"
#include "lib/ansi.h"

namespace pingpong {
	class irc {
		private:
			std::mutex console_mux = std::mutex();

		public:
			static constexpr int default_port = 6667;
			static std::string default_nick, default_user, default_realname;

			std::string username, realname;

			/** A map of server IDs to server instances. */
			std::map<std::string, server *> servers {};

			std::list<server *> server_order {};

			server *active_server = nullptr;

			irc(std::string user, std::string real): username(user), realname(real) {}
			irc(): irc(default_user, default_realname) {}
			~irc();

			/** Finds and returns the server with a given ID or nullptr if none is found. */
			server * get_server(const std::string &id) const;

			/** Returns whether a server with a given ID exists. */
			bool has_server(const std::string &id) const;

			/** Returns whether this instance contains a given server pointer. */
			bool has_server(server *) const;

			/** If this server contains a given server pointer, this function returns its key. Otherwise, it returns an
			 *  empty string. */
			std::string get_key(server *) const;
			
			std::unique_lock<std::mutex> lock_console() { return std::unique_lock(console_mux); }
			void init();
			void init_messages();

			/** Creates a unique ID for a server, a hostname. */
			std::string create_id(const std::string &hostname);

			irc & operator+=(server *);
			irc & operator-=(server *);
	};
}

#endif
