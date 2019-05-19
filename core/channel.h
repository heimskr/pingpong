#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <string>

namespace pingpong {
	using std::string;
	class server;

	class channel {
		public:
			string name;
			std::shared_ptr<server> serv;

			channel(string, std::shared_ptr<server>);
			channel(string);
			bool is_user();
	};
}

#endif
