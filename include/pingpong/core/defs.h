#ifndef PINGPONG_CORE_DEFS_H_
#define PINGPONG_CORE_DEFS_H_

#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

namespace pingpong {
	class channel;
	class server;
	class irc;
	class user;
	class message;

	enum class hat: char {none=' ', voiced='+', halfop='%', op='@', admin='&', owner='~'};
	extern std::unordered_map<hat, int> hat_ranks;
	extern std::unordered_map<char, hat> hat_map;

	bool operator<(hat, hat);
	bool operator>(hat, hat);

	class user_exists_error: public std::exception {
		public:
			server *serv;
			std::string old_nick, new_nick;
			user_exists_error(server *serv_, const std::string &old_, const std::string &new_):
				serv(serv_), old_nick(old_), new_nick(new_) {}
			const char * what() const throw() { return "A user with the same name already exists"; }
	};
}

#ifdef VSCODE
long strtol(const char *, char **, int); // suppresses an incorrect VS Code error
#endif

#endif
