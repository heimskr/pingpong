#ifndef PINGPONG_COMMANDS_CAP_H_
#define PINGPONG_COMMANDS_CAP_H_

#include <unordered_set>

#include "pingpong/commands/command.h"
#include "pingpong/core/features.h"

namespace pingpong {
	class cap_command: public command {
		public:
			enum class action {none, ls, end, req};

			std::unordered_set<features::type> to_request {};
			action type = action::none;

			template <typename Iter>
			cap_command(server *serv_, Iter begin, Iter end, action type_ = action::none):
				command(serv_), to_request(begin, end), type(type_) {}

			template <typename Container>
			cap_command(server *serv_, const Container &cont, action type_ = action::none):
				cap_command(serv_, cont.begin(), cont.end(), type_) {}

			cap_command(server *serv_, std::initializer_list<features::type> ilist = {}, action type_ = action::none):
				cap_command(serv_, ilist.begin(), ilist.end(), type_) {}

			cap_command(server *serv_, action type_):
				command(serv_), type(type_) {}

			operator std::string() const override;
	};
}

#endif
