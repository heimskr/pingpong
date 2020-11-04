#ifndef PINGPONG_COMMANDS_CAP_H_
#define PINGPONG_COMMANDS_CAP_H_

#include <unordered_set>

#include "pingpong/commands/Command.h"
#include "pingpong/core/Features.h"

namespace PingPong {
	class CapCommand: public Command {
		public:
			enum class Action {None, Ls, End, Req};

			std::unordered_set<Features::Type> to_request {};
			Action type = Action::None;

			template <typename Iter>
			CapCommand(Server *server_, Iter begin, Iter end, Action type_ = Action::None):
				Command(server_), to_request(begin, end), type(type_) {}

			template <typename Container>
			CapCommand(Server *server_, const Container &cont, Action type_ = Action::None):
				CapCommand(server_, cont.begin(), cont.end(), type_) {}

			CapCommand(Server *server_, std::initializer_list<Features::Type> ilist = {}, Action type_ = Action::None):
				CapCommand(server_, ilist.begin(), ilist.end(), type_) {}

			CapCommand(Server *server_, Action type_):
				Command(server_), type(type_) {}

			operator std::string() const override;
	};
}

#endif
