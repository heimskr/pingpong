#include <iostream>

#include "../core/channel.h"

using namespace std;

namespace tests {
	void test_channel() {
		cout << "hi" << endl;
	}
}

int main(int, char **) {
	tests::test_channel();
	return 0;
}