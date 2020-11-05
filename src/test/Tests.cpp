#include <functional>
#include <iostream>
#include <string>

#include "pingpong/core/Channel.h"
#include "pingpong/core/Server.h"

#include "pingpong/commands/Join.h"
#include "pingpong/commands/Nick.h"
#include "pingpong/commands/Privmsg.h"
#include "pingpong/commands/User.h"

#include "pingpong/events/Event.h"
#include "pingpong/events/Join.h"

using namespace std;
using namespace PingPong;

namespace tests {
	void test_channel() {
		Channel chan("#programming");
		Channel user("NickServ");
	}

	void test_commands(Server *server) {
		UserCommand(server, "some_user", "Some Name").send();
		NickCommand(server, "somenick").send();
		PrivmsgCommand(server, "#channel", "Hello, world!").send();
		JoinCommand(server, vector<JoinPair>({{"#foo", ""}, {"#bar", "B4R"}})).send();
		JoinCommand(server, vector<JoinPair>({{"#baz", ""}, {"#quux", ""}})).send();
	}

	void test_network(Server &server) {
		server.start();
		server.setNick("pingpong");
		server.worker.join();
	}

	void test_mask() {
		string mstr = "nick!user@host";
		Mask m(mstr);
		cout << "Nick[" << m.nick << "], User[" << m.user << "], Host[" << m.host << "]\n";
	}

	void test_events(Server &server) {
		Server *ptr = &server;

		Events::listen<JoinEvent>([&](auto *ev) {
			cout << "join1(" << *ev->who << " -> " << *ev->channel << ")\n";
		});

		Events::listen<JoinEvent>([&](auto *ev) {
			cout << "join2(" << *ev->who << " -> " << *ev->channel << ")\n";
		});

		for (int i = 1; i < 10; ++i) {
			std::shared_ptr<User> u = std::make_shared<User>("someone" + std::to_string(i), ptr);
			std::shared_ptr<Channel> c = std::make_shared<Channel>("#somewhere" + std::to_string(i), ptr);
			Events::dispatch<JoinEvent>(u, c);
		}
	}
}

int main(int, char **) {
	IRC *instance = new IRC();
	Server server(instance, "localhost");

	// tests::test_network(serv);
	tests::test_events(server);
	delete instance;
	return 0;
}
