#ifndef PINGPONG_CORE_PPDEFS_H_
#define PINGPONG_CORE_PPDEFS_H_

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

	class user_exists_error: public std::exception {
		public:
			server *serv;
			std::string old_nick, new_nick;
			user_exists_error(server *serv_, const std::string &old_, const std::string &new_):
				serv(serv_), old_nick(old_), new_nick(new_) {}
			const char * what() const throw() { return "A user with the same name already exists"; }
	};
}

#endif
