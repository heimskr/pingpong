#include <iostream>

#include "core/all.h"
#include "commands/all.h"

using namespace std;
using namespace pingpong;

// namespace pingpong {
// 	void server::quote(const string &raw) {
// 		cout << "Quote(\"" << raw << "\")" << endl;
// 	}
// }

namespace tests {

	void test_channel() {
		channel chan("#programming");
		channel user("NickServ");
		cout << "chan.is_user(): " << chan.is_user() << endl;
		cout << "user.is_user(): " << user.is_user() << endl;
	}

	void test_commands(server_ptr serv) {
		user_command(serv, "some_user", "Some Name").send();
		nick_command(serv, "somenick").send();
		privmsg_command(serv, "#channel", "Hello, world!").send();
		join_command(serv, vector<join_pair>({{"#foo", ""}, {"#bar", "B4R"}})).send();
		join_command(serv, vector<join_pair>({{"#baz", ""}, {"#quux", ""}})).send();
	}

	void test_network(server &serv) {
		serv.start();
		serv.server_thread->join();
	}
}

int main(int, char **) {
	irc instance;
	server serv(instance, "localhost");

	tests::test_network(serv);
	return 0;
}
