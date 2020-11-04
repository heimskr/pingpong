#include <cstring>
#include <netdb.h>

#include "pingpong/net/NetError.h"

namespace PingPong::Net {
	const char * net_error::what() const throw() {
		return strerror(status_code);
	}
}
