#include <cstring>
#include <netdb.h>

#include "pingpong/net/NetError.h"

namespace PingPong::Net {
	const char * NetError::what() const throw() {
		return strerror(statusCode);
	}
}
