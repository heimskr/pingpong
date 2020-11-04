#ifndef PINGPONG_MESSAGES_ERROR_H_
#define PINGPONG_MESSAGES_ERROR_H_

#include "Basic.h"

namespace PingPong {
	class ErrorMessage: public BasicMessage {
		public:
			ErrorMessage(PingPong::Line line_): BasicMessage(line_) {}

			static constexpr auto getName = []() -> std::string { return "ERROR"; };

			operator std::string() const override;
			bool operator()(Server *) override;
	};
}

#endif
