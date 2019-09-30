#ifndef PINGPONG_NET_RESOLUTION_ERROR_H_
#define PINGPONG_NET_RESOLUTION_ERROR_H_

#include <stdexcept>

namespace pingpong::net {
	class resolution_error: public std::runtime_error {
		public:
			int status_code;

			resolution_error(int status_code_): std::runtime_error("Resolution failed"), status_code(status_code_) {}

			virtual const char * what() const throw() override;
	};
}

#endif
