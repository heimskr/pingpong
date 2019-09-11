#include <string>
#include "commands/command.h"
	
namespace pingpong {
	void command::send() {
		serv->quote(std::string(*this));
	}
}
