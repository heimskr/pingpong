#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net/sock.h"
#include "net/net_error.h"
#include "net/resolution_error.h"

#include "lib/ansi.h"

namespace pingpong::net {
	sock::sock(const std::string &hostname_, int port_): hostname(hostname_), port(port_) {
		struct addrinfo hints = {};
		std::memset(&hints, 0, sizeof(hints));
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
		net_fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		int status = ::connect(net_fd, info->ai_addr, info->ai_addrlen);
		if (status != 0)
			throw net_error(status);
		connected = true;

		control_fd = socket(AF_UNIX, SOCK_STREAM, 0);
		FD_ZERO(&fds);
		FD_SET(net_fd, &fds);
		FD_SET(control_fd, &fds);
	}

	void sock::close() {
		DBG("sock::close()");
		control_message message = control_message::close;
		::send(control_fd, &message, 1, 0);
	}

	ssize_t sock::send(const void *data, size_t bytes) {
		if (!connected)
			throw std::invalid_argument("Socket not connected");
		return ::send(net_fd, data, bytes, 0);
	}

	ssize_t sock::recv(void *data, size_t bytes) {
		if (!connected)
			throw std::invalid_argument("Socket not connected");

		// int out = select(std::max(net_fd, control_fd) + 1, &fds, NULL, NULL, NULL);
		fd_set fds_copy = fds;
		int status = select(FD_SETSIZE, &fds_copy, NULL, NULL, NULL);
		if (status < 0) {
			DBG("select status: " << status);
			return status;
		}
			
		if (FD_ISSET(net_fd, &fds_copy)) {
			DBG("Reading from net_fd.");
			return ::recv(net_fd, data, bytes, 0);
		} else if (FD_ISSET(control_fd, &fds_copy)) {
			DBG("Reading from control_fd.");
			control_message message;
			status = ::recv(control_fd, &message, 1, 0);
			if (status < 0) {
				DBG("control_fd status: " << status);
				return status;
			}

			if (message == control_message::close) {
				DBG("Received close message.");
				close();
				return 0;
			} else {
				DBG("Unknown control message: '" << static_cast<char>(message) << "'");
			}
		} else {
			DBG("???");
		}

		DBG("This is bad.");
		return 0;
		
		// ssize_t out = ::recv(net_fd, data, bytes, 0);
		// return out;
	}
}
