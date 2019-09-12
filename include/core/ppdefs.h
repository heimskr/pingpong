#ifndef CORE_DEFS_H_
#define CORE_DEFS_H_

#include <exception>
#include <memory>
#include <string>

namespace pingpong {
	class channel;
	class server;
	class irc;
	class user;
	class message;

	enum class hat: char {none=' ', voiced='+', halfop='%', op='@', admin='&', owner='~'};

	using  server_ptr = pingpong::server *;
	using channel_ptr = std::shared_ptr<channel>;
	using    user_ptr = std::shared_ptr<user>;
	using message_ptr = std::shared_ptr<message>;

	class user_exists_error: public std::exception {
		public:
			server_ptr serv;
			std::string old_nick, new_nick;
			user_exists_error(server_ptr serv_, const std::string &old_, const std::string &new_):
				serv(serv_), old_nick(old_), new_nick(new_) {}
			const char * what() const throw() { return "A user with the same name already exists"; }
	};
}

#endif
