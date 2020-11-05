#ifndef COMMANDS_JOIN_H_
#define COMMANDS_JOIN_H_

#include <string>
#include <vector>

#include "Command.h"

namespace PingPong {
	using JoinPair = std::pair<std::string, std::string>;

	class JoinCommand: public Command {
		public:
			std::vector<JoinPair> pairs;

			JoinCommand(Server *server_, const std::vector<JoinPair> &pairs_):
				Command(server_), pairs(pairs_) {}

			JoinCommand(Server *server_, const std::string &chan):
				JoinCommand(server_, {JoinPair(chan, "")}) {}

			JoinCommand(const Channel &chan);

			JoinCommand(Server *, const std::vector<std::string> &);
			JoinCommand(Server *, const std::vector<Channel> &);

			operator std::string() const override;
			bool send() override;
	};
}

#endif
