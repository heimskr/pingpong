#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>

#include "pingpong/net/SocketBuffer.h"
#include "pingpong/net/NetError.h"

#include "lib/formicine/ansi.h"

#if VSCODE
#define EOF 0
namespace std { void * memmove(void *dest, const void *src, std::size_t count); }
#endif

namespace PingPong::Net {
	SocketBuffer::SocketBuffer(Sock *source_, size_t buffer_size, size_t putback_size):
	source(source_), bufferSize(buffer_size), putbackSize(putback_size) {
		buffer = new char[bufferSize];
		setg(buffer + putbackSize, buffer + putbackSize, buffer + putbackSize);
	}

	SocketBuffer::~SocketBuffer() {
		delete[] buffer;
	}

	std::streambuf::int_type SocketBuffer::overflow(std::streambuf::int_type byte) {
		if (!traits_type::eq_int_type(byte, traits_type::eof())) {
			try {
				source->send(&byte, 1);
			} catch (const NetError &err) {
				DBG("Network error in overflow(): " << err.what());
				return EOF;
			}
		}

		return traits_type::not_eof(byte);
	}

	std::streamsize SocketBuffer::xsputn(const char *src, std::streamsize size) {
		try {
			return source->send(src, size);
		} catch (const NetError &err) {
			DBG("Network error in xsputn(): " << err.what());
			return EOF;
		}
	}

	std::streambuf::int_type SocketBuffer::underflow() {
		if (gptr() < egptr())
			return traits_type::to_int_type(*gptr());

		size_t putback = std::min(putbackSize, static_cast<size_t>(gptr() - eback()));
		std::memmove(buffer + putbackSize - putback, gptr() - putback, putback);

		ssize_t bytes_read;
		try {
			bytes_read = source->recv(buffer + putbackSize, bufferSize - putbackSize);
		} catch (const NetError &err) {
			DBG("Network error in underflow(): " << err.what());
			return EOF;
		}

		if (bytes_read == 0)
			return EOF;

		setg(buffer + putbackSize - putback, buffer + putbackSize, buffer + putbackSize + bytes_read);
		return traits_type::to_int_type(*gptr());
	}

	void SocketBuffer::close() {
		source->close();
	}
}
