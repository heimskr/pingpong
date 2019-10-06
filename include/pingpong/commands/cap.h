#ifndef PINGPONG_COMMANDS_CAP_H_
#define PINGPONG_COMMANDS_CAP_H_

#include <unordered_set>

#include "pingpong/commands/command.h"
#include "pingpong/core/features.h"

namespace pingpong {
	class cap_command: public command {
		public:
			std::unordered_set<features::type> to_request;

			template <typename Iter>
			cap_command(server *serv_, Iter begin, Iter end):
				command(serv_), to_request(begin, end) {}

			template <typename Container>
			cap_command(server *serv_, const Container &cont):
				cap_command(serv_, cont.begin(), cont.end()) {}

			cap_command(server *serv_, std::initializer_list<features::type> ilist = {}):
				cap_command(serv_, ilist.begin(), ilist.end()) {}

			operator std::string() const override;
	};
}

#endif
