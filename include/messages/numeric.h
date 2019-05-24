#ifndef MESSAGES_NUMERIC_H_
#define MESSAGES_NUMERIC_H_

#include <stdexcept>
#include <string>

#include "core/channel.h"
#include "messages/message.h"
#include "messages/sourced.h"

namespace pingpong {
	class numeric_message: public message {
		public:
			using names = std::tuple<std::string, channel::visibility, std::vector<std::pair<user::hat, std::string>>>;

			long number;

			numeric_message(pingpong::line);

			operator std::string() const override;
			void operator()(server_ptr) const override;

			static constexpr auto get_name = []() -> std::string { return "_NUMERIC"; };
			static bool is_numeric(const char *);
			static bool is_numeric(const char *, long &);
			static names parse353(const std::string &);
	};
}

#endif
