#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "net/sock.h"
#include "net/net_error.h"
#include "net/resolution_error.h"

namespace pingpong::net {
	sock::sock(const std::string &hostname_, int port_): hostname(hostname_), port(port_) {
		struct addrinfo hints {0};
		hints.ai_family   = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		int status = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &info);
		if (status != 0)
			throw resolution_error(status);
	}
	
	sock::~sock() {
		freeaddrinfo(info);
	}

	void sock::connect() {
		fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		int status = ::connect(fd, info->ai_addr, info->ai_addrlen);
		if (status != 0)
			throw net_error(status);
	}
}
