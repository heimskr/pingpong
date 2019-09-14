#ifndef PINGPONG_NET_SOCKET_STREAM_H_
#define PINGPONG_NET_SOCKET_STREAM_H_

#include <iostream>
#include <vector>

#include "net/sock.h"

namespace pingpong::net {
	// socket_buffer uses code by Edd Dawson distributed under the Boost Software License, Version 1.0:
	// http://www.mr-edd.co.uk/static/blog/beginners_guide_streambuf/downloads/example_code.zip
	class socket_buffer: public std::streambuf {
		private:
			sock *source;
			std::vector<char> buffer;
			const size_t put_back;

			std::streambuf::int_type underflow();
		
		public:
			socket_buffer(sock *source_, size_t bufsize = 1024, size_t put_back_ = 8);
	};

	class socket_stream: public std::iostream {
		private:
			sock *source;

		public:
			// socket_stream(sock *source_): source(source_) {}
			
			socket_stream(const socket_stream &) = delete;
			socket_stream & operator=(const socket_stream &) = delete;

			socket_stream & operator<<(const std::string &);

			template <typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
			socket_stream & operator<<(T num) {
				return *this << std::to_string(num);
			}

			template <typename T>
			socket_stream & operator<<(const T &obj) {
				return *this << std::string(obj);
			}
	};
}

#endif
