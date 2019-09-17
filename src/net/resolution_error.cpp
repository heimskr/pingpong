#include <netdb.h>

#include "net/resolution_error.h"

namespace pingpong::net {
	const char * resolution_error::what() const throw() {
		return gai_strerror(status_code);
	}
}