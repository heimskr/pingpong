#ifndef PINGPONG_NET_SEND_ERROR_H_
#define PINGPONG_NET_SEND_ERROR_H_

#include <stdexcept>

namespace pingpong::net {
	class net_error: public std::runtime_error {
		public:
			int status_code;

			net_error(int status_code_): std::runtime_error("Network operation failed"), status_code(status_code_) {}

			virtual const char * what() const throw() override;
	};
}

#endif
