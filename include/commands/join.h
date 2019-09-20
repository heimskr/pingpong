#ifndef COMMANDS_JOIN_H_
#define COMMANDS_JOIN_H_

#include <string>
#include <vector>

#include "command.h"

namespace pingpong {
	using join_pair = std::pair<std::string, std::string>;

	class join_command: public command {
		public:
			std::vector<join_pair> pairs;

			join_command(channel);
			join_command(server *, std::string);
			join_command(server *, std::vector<join_pair>);
			join_command(server *, std::vector<std::string>);
			join_command(server *, std::vector<channel>);

			operator std::string() const override;
			void send() override;
	};
}

#endif
