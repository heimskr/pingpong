#ifndef MESSAGES_NUMERIC_H_
#define MESSAGES_NUMERIC_H_

#include <stdexcept>
#include <string>

#include "core/channel.h"
#include "messages/message.h"
#include "messages/sourced.h"

namespace pingpong {
	class numeric_message: public message {
		using names = std::tuple<std::string, channel::visibility, std::vector<std::pair<hat, std::string>>>;

		public:
			long number;

			numeric_message(pingpong::line);

			static constexpr auto get_name = []() -> std::string { return "_NUMERIC"; };
			virtual std::string name() const override { return get_name(); }

			operator std::string() const override;
			void operator()(server_ptr) const override;

			static bool is_numeric(const char *);
			static bool is_numeric(const char *, long &);
			static names parse353(const std::string &);
	};
}

#endif
