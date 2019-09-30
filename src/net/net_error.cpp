#include <cstring>
#include <netdb.h>

#include "pingpong/net/net_error.h"

namespace pingpong::net {
	const char * net_error::what() const throw() {
		return strerror(status_code);
	}
}
