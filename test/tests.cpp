#include <iostream>

#include "../core/all.h"
#include "../commands/all.h"

using namespace std;

namespace pingpong {
	void server::quote(string raw) {
		cout << "Quote(\"" << raw << "\")" << endl;
	}
}

namespace tests {
	using namespace pingpong;

	const serv_ptr serv = make_shared<server>("irc.subluminal.net");

	void test_channel() {
		channel chan("#programming");
		channel user("NickServ");
		cout << "chan.is_user(): " << chan.is_user() << endl;
		cout << "user.is_user(): " << user.is_user() << endl;
	}

	void test_commands() {
		user_command usercmd(serv, "some_user", "Some Name");
		usercmd.send();

		nick_command nickcmd(serv, "somenick");
		nickcmd.send();

		privmsg_command privmsgcmd(serv, "#channel", "Hello, world!");
		privmsgcmd.send();
	}
}

int main(int, char **) {
	tests::test_commands();
	return 0;
}