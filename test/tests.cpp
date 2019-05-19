#include <iostream>

#include "../core/channel.h"

using namespace std;

namespace tests {
	void test_channel() {
		pingpong::channel chan("#programming");
		pingpong::channel user("NickServ");
		cout << "chan.is_user(): " << chan.is_user() << endl;
		cout << "user.is_user(): " << user.is_user() << endl;
	}
}

int main(int, char **) {
	tests::test_channel();
	return 0;
}