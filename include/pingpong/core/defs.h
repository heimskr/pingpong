#ifndef PINGPONG_CORE_DEFS_H_
#define PINGPONG_CORE_DEFS_H_

#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

namespace PingPong {
	class Channel;
	class Server;
	class IRC;
	class User;
	class Message;

	enum class Hat: char {None=' ', Voiced='+', Halfop='%', Op='@', Admin='&', Owner='~'};

	struct UserExistsError: public std::exception {
		Server *server;
		std::string oldNick, newNick;
		UserExistsError(Server *server_, const std::string &old_, const std::string &new_):
			server(server_), oldNick(old_), newNick(new_) {}
		const char * what() const throw() { return "A user with the same name already exists"; }
	};
}

#ifdef VSCODE
long strtol(const char *, char **, int); // suppresses an incorrect VS Code error
#endif

#endif
