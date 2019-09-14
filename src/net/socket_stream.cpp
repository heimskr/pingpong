#include <cerrno>
#include <cstdio>
#include <cstring>

#include "net/socket_stream.h"
#include "net/net_error.h"

namespace pingpong::net {
	socket_buffer::socket_buffer(sock *source_, size_t bufsize, size_t put_back_):
	source(source_), put_back(put_back), buffer(std::max(bufsize, put_back) + put_back) {
		char *end = &*buffer.end();
		setg(end, end, end);
	}

	std::streambuf::int_type socket_buffer::underflow() {
		if (gptr() < egptr())
			return traits_type::to_int_type(*gptr());
		
		char *base = &buffer.front();
		char *start = base;

		if (eback() == base) {
			std::memmove(base, egptr() - put_back, put_back);
			start += put_back;
		}

		size_t n = recv(source->fd, start, buffer.size() - (start - base), 0);

		if (n == 0)
			return traits_type::eof();
		else if (n == -1)
			throw net_error(errno);

		setg(base, start, start + n);
		return traits_type::to_int_type(*gptr());
	}

	// socket_stream & socket_stream::operator<<(const std::string &str) {
	// 	ssize_t sent = send(source->fd, str.c_str(), str.length(), 0);
	// 	if (sent == -1)
	// 		throw net_error(errno);
	// }
}
