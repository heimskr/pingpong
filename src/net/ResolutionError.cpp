#include <netdb.h>

#include "pingpong/net/ResolutionError.h"

namespace PingPong::Net {
	const char * ResolutionError::what() const throw() {
		return gai_strerror(statusCode);
	}
}
