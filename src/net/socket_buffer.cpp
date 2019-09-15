#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>

#include "net/socket_buffer.h"
#include "net/net_error.h"

#include "lib/ansi.h"

namespace pingpong::net {
	socket_buffer::socket_buffer(sock *source_, size_t buffer_size_, size_t putback_size_):
	source(source_), buffer_size(buffer_size_), putback_size(putback_size_) {
		buffer = new char[buffer_size];
		setg(buffer + putback_size, buffer + putback_size, buffer + putback_size);
	}

	socket_buffer::~socket_buffer() {
		delete[] buffer;
	}



	std::streambuf::int_type socket_buffer::overflow(std::streambuf::int_type byte) {
		if (!traits_type::eq_int_type(byte, traits_type::eof()))
			source->send(&byte, 1);
		return traits_type::not_eof(byte);
	}

	std::streamsize socket_buffer::xsputn(const char *src, std::streamsize size) {
		return source->send(src, size);
	}



	std::streambuf::int_type socket_buffer::underflow() {
		if (gptr() < egptr())
			return traits_type::to_int_type(*gptr());

		int putback = std::min(putback_size, static_cast<size_t>(gptr() - eback()));
		std::memmove(buffer + putback_size - putback, gptr() - putback, putback);

		int bytes_read = source->recv(buffer + putback_size, buffer_size - putback_size);
		if (bytes_read == 0)
			return EOF;

		setg(buffer + putback_size - putback, buffer + putback_size, buffer + putback_size + bytes_read);
		return traits_type::to_int_type(*gptr());
	}
}
