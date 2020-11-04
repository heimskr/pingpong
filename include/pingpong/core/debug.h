#ifndef CORE_DEBUG_H_
#define CORE_DEBUG_H_

#include "pingpong/core/IRC.h"

#include "lib/formicine/ansi.h"

#define YIKES(x) DBG(ansi::color::red    << " !! " << ansi::action::reset << x)
#define WARN(x)  DBG(ansi::color::yellow << " ?? " << ansi::action::reset << x)

namespace PingPong {
	class Debug {
		public:
			static void printAll(const IRC &);
	};
};

#endif
