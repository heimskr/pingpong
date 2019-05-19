#ifndef CORE_IRC_H_
#define CORE_IRC_H_

namespace pingpong { class server; }

#include <string>
#include <vector>

namespace pingpong {
	struct line {
		std::string tags, source, command, parameters;
		line(std::string in);
	};

	class irc {
		private:
			std::vector<server> servers;

		public:
			static constexpr int default_port = 6667;
	};
}

#endif
