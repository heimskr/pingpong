#ifndef CORE_DEBUG_H_
#define CORE_DEBUG_H_

#include "lib/ansi.h"
#include "core/irc.h"

#define YIKES(x) \
	(ansi::ansistream() << ansi::red << " !! " << ansi::reset << x << "\n")

#define WARN(x) \
	(ansi::ansistream() << ansi::yellow << " ?? " << ansi::reset << x << "\n")

#define HELLO std::cout << "\e[2m[" << __FILE__ << ":" << __LINE__ << "]\e[0m\n"

namespace pingpong {
	class debug {
		public:
			static void print_all(const irc &);
	};
};

#endif
