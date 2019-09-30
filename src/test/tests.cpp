#include <functional>
#include <iostream>
#include <string>

#include "pingpong/core/channel.h"
#include "pingpong/core/server.h"

#include "pingpong/commands/join.h"
#include "pingpong/commands/nick.h"
#include "pingpong/commands/privmsg.h"
#include "pingpong/commands/user.h"

#include "pingpong/events/event.h"
#include "pingpong/events/join.h"

using namespace std;
using namespace pingpong;

namespace tests {
	void test_channel() {
		channel chan("#programming");
		channel user("NickServ");
	}

	void test_commands(server *serv) {
		user_command(serv, "some_user", "Some Name").send();
		nick_command(serv, "somenick").send();
		privmsg_command(serv, "#channel", "Hello, world!").send();
		join_command(serv, vector<join_pair>({{"#foo", ""}, {"#bar", "B4R"}})).send();
		join_command(serv, vector<join_pair>({{"#baz", ""}, {"#quux", ""}})).send();
	}

	void test_network(server &serv) {
		serv.start();
		serv.set_nick("pingpong");
		serv.worker.join();
	}

	void test_mask() {
		string mstr = "nick!user@host";
		mask m(mstr);
		cout << "Nick[" << m.nick << "], User[" << m.user << "], Host[" << m.host << "]\n";
	}

	void test_events(server &serv) {
		server *ptr = &serv;

		events::listen<join_event>([&](auto *ev) {
			cout << "join1(" << *ev->who << " -> " << *ev->chan << ")\n";
		});

		events::listen<join_event>([&](auto *ev) {
			cout << "join2(" << *ev->who << " -> " << *ev->chan << ")\n";
		});

		for (int i = 1; i < 10; ++i) {
			std::shared_ptr<user> u = std::make_shared<user>("someone" + std::to_string(i), ptr);
			std::shared_ptr<channel> c = std::make_shared<channel>("#somewhere" + std::to_string(i), ptr);
			events::dispatch<join_event>(u, c);
		}
	}
}

int main(int, char **) {
	irc *instance = new irc();
	server serv(instance, "localhost");

	// tests::test_network(serv);
	tests::test_events(serv);
	delete instance;
	return 0;
}
