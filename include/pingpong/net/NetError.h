#ifndef PINGPONG_NET_SEND_ERROR_H_
#define PINGPONG_NET_SEND_ERROR_H_

#include <stdexcept>

namespace PingPong::Net {
	class NetError: public std::runtime_error {
		public:
			int statusCode;

			NetError(int status_code): std::runtime_error("Network operation failed"), statusCode(status_code) {}

			virtual const char * what() const throw() override;
	};
}

#endif
