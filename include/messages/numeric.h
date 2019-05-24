#ifndef MESSAGES_NUMERIC_H_
#define MESSAGES_NUMERIC_H_

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class numeric_message: public message {
		public:
			static constexpr auto get_name = []() -> std::string { return "_NUMERIC"; };
			static bool is_numeric(const char *);
			static bool is_numeric(const char *, long &);
			static std::pair<std::string, std::unordered_set<std::string>> parse353(const std::string &);

			long number;

			numeric_message(pingpong::line);

			operator std::string() const override;
			void operator()(server_ptr) const override;
	};
}

#endif
