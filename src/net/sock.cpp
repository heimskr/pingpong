#include <cstring>
#include <cerrno>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "pingpong/net/sock.h"
#include "pingpong/net/net_error.h"
#include "pingpong/net/resolution_error.h"

#include "lib/formicine/ansi.h"

namespace pingpong::net {
	int sock::sock_count = 0;

	sock::sock(const std::string &hostname_, int port_): hostname(hostname_), port(port_) {
		struct addrinfo hints = {};
		std::memset(&hints, 0, sizeof(hints));
		hints.ai_family   = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		int status = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &info);
		if (status != 0)
			throw resolution_error(errno);
	}
	
	sock::~sock() {
		freeaddrinfo(info);
	}

	void sock::connect() {
		net_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		int status = ::connect(net_fd, info->ai_addr, info->ai_addrlen);
		if (status != 0) {
			DBG("connect(): " << strerror(errno));
			throw net_error(errno);
		}

		int control_pipe[2];
		status = pipe(control_pipe);
		if (status != 0) {
			DBG("pipe(): " << strerror(errno));
			throw net_error(errno);
		}

		control_read = control_pipe[0];
		control_write = control_pipe[1];

		FD_ZERO(&fds);
		FD_SET(net_fd, &fds);
		FD_SET(control_read, &fds);

		connected = true;
	}

	void sock::close() {
		control_message message = control_message::close;
		::write(control_write, &message, 1);
	}

	ssize_t sock::send(const void *data, size_t bytes) {
		if (!connected)
			throw std::invalid_argument("Socket not connected");
		return ::send(net_fd, data, bytes, 0);
	}

	ssize_t sock::recv(void *data, size_t bytes) {
		if (!connected)
			throw std::invalid_argument("Socket not connected");

		fd_set fds_copy = fds;
		int status = select(FD_SETSIZE, &fds_copy, NULL, NULL, NULL);
		if (status < 0) {
			DBG("select status: " << strerror(status));
			throw net_error(errno);
		}
			
		if (FD_ISSET(net_fd, &fds_copy)) {
			return ::recv(net_fd, data, bytes, 0);
		} else if (FD_ISSET(control_read, &fds_copy)) {
			control_message message;
			status = ::read(control_read, &message, 1);
			if (status < 0) {
				DBG("control_fd status: " << strerror(status));
				throw net_error(errno);
			}

			if (message == control_message::close) {
				close();
			} else {
				DBG("Unknown control message: '" << static_cast<char>(message) << "'");
			}

			return 0;
		} else {
			DBG("No file descriptor is ready.");
		}

		return -1;
	}
}
