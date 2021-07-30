#pragma once

#include "Command.h"

namespace PingPong {
	class PassCommand: public Command {
		public:
			std::string password;

			PassCommand(Server *server_, const std::string &password_): Command(server_), password(password_) {}
			operator std::string() const override;
			bool isSilent() const override { return true; }
	};
}
