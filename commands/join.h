#ifndef COMMANDS_JOIN_H_
#define COMMANDS_JOIN_H_

#include <string>
#include <utility>
#include <vector>

#include "command.h"

namespace pingpong {
	using std::string;

	using join_pair = std::pair<channel, string>;

	class join_command: public command {
		public:
			std::vector<join_pair> pairs;

			join_command(channel);
			join_command(serv_ptr, string);
			join_command(serv_ptr, std::vector<join_pair>);
			join_command(serv_ptr, std::vector<string>);
			join_command(serv_ptr, std::vector<channel>);

			virtual string to_string() const;
	};
}

#endif
