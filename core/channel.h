#ifndef CORE_CHANNEL_H_
#define CORE_CHANNEL_H_

#include <string>

namespace pingpong {
	using std::string;

	class channel {
		public:
			string name;

			channel(string name_);
			bool is_user();
	};
}

#endif
