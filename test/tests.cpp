#include <iostream>

#include "../core/channel.h"
#include "../core/server.h"
#include "../core/irc.h"
#include "../commands/user.h"
#include "../commands/nick.h"

using namespace std;

namespace pingpong {
	void server::quote(string raw) {
		cout << "Quote(\"" << raw << "\")" << endl;
	}
}

namespace tests {
	using namespace pingpong;

	void test_channel() {
		channel chan("#programming");
		channel user("NickServ");
		cout << "chan.is_user(): " << chan.is_user() << endl;
		cout << "user.is_user(): " << user.is_user() << endl;

		serv_ptr serv = make_shared<server>("irc.subluminal.net");
		user_command usercmd(serv, "some_user", "Some Name");
		usercmd.send();

		nick_command nickcmd(serv, "somenick");
		nickcmd.send();
	}
}

int main(int, char **) {
	tests::test_channel();
	return 0;
}