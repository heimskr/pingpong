#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <string>

namespace pingpong {
	class channel {
		public:
			std::string name;

			channel(std::string name_): name(name_) { }
			bool is_user();
	};
}

#endif
