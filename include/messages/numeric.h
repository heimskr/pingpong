#ifndef MESSAGES_NUMERIC_H_
#define MESSAGES_NUMERIC_H_

#include <stdexcept>
#include <string>
#include <unordered_set>

#include "core/channel.h"
#include "core/numeric_type.h"
#include "messages/message.h"
#include "messages/sourced.h"

namespace pingpong {
	class numeric_message: public message {
		using names = std::tuple<std::string, channel::visibility, std::vector<std::pair<hat, std::string>>>;

		private:
			static std::unordered_set<numeric_type> types;

			inline int to_int() const { return static_cast<int>(type); }
			inline std::string to_string() const { return std::to_string(to_int()); }

		public:

			numeric_type type;

			numeric_message(pingpong::line);

			static constexpr auto get_name = []() -> std::string { return "_NUMERIC"; };

			operator std::string() const override;
			bool operator()(server *) override;

			static bool is_numeric(const char *);
			static bool is_numeric(const char *, int &);
			static names parse353(const std::string &);

			bool operator==(int n) const;
			bool operator!=(int n) const;
			bool operator==(numeric_type type_) const;
			bool operator!=(numeric_type type_) const;
	};
}

#endif
