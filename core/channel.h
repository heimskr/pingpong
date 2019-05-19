#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <string>

namespace pingpong {
	class server;

	class channel {
		public:
			std::string name;
			std::shared_ptr<server> serv;

			channel(std::string, std::shared_ptr<server>);
			channel(std::string);
			bool is_user();

			operator std::string() const { return name; }
	};
}

#endif
