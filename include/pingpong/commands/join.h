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

			join_command(server *serv_, const std::vector<join_pair> &pairs_):
				command(serv_), pairs(pairs_) {}

			join_command(const channel &chan):
				join_command(chan.serv, {join_pair(chan, "")}) {}

			join_command(server *serv_, const std::string &chan):
				join_command(serv_, {join_pair(chan, "")}) {}

			join_command(server *, const std::vector<std::string> &);
			join_command(server *, const std::vector<channel> &);

			operator std::string() const override;
			bool send() override;
	};
}

#endif
