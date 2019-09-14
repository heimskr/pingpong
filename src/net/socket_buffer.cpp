#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>

#include "net/socket_buffer.h"
#include "net/net_error.h"

namespace pingpong::net {
	std::streamsize socket_buffer::xsputn(const char *src, std::streamsize size) {
		return source->send(src, size);
	}

	std::streamsize socket_buffer::xsgetn(char *dest, std::streamsize size) {
		return source->recv(dest, size);
	}

	std::streambuf::int_type socket_buffer::overflow(std::streambuf::int_type byte) {
		return source->send(&byte, 1);
	}

	std::streambuf::int_type socket_buffer::underflow() {
		std::streambuf::int_type byte;
		if (source->recv(&byte, 1))
			return traits_type::eof();
		return byte;
	}
}
