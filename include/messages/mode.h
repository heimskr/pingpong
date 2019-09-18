#ifndef PINGPONG_MESSAGES_MODE_H_
#define PINGPONG_MESSAGES_MODE_H_

#include <unordered_set>

#include "message.h"

namespace pingpong {
	class mode_message: public message {
		private:
			/** Validates the string of additions/removals with a touch of paranoia. */
			void validate_modes(const std::string &, const pingpong::line &);

		public:
			enum class mode_type {self, channel};
			
			/** There are two types of mode changes: user modes and channel modes. This indicates which type applies. */
			mode_type type;

			/** The raw string of mode changes, such as "-v+o". */
			std::string modes;

			/** A set of all modes indicated as added by the message. */
			std::unordered_set<char> added = {};

			/** A set of all modes indicated as removed by the message. */
			std::unordered_set<char> removed = {};

			mode_message(const pingpong::line &line_);

			/** Applies the changes to the relevant channel (for channel mode changes) or to you (for user mode
			 *  changes). */
			void apply();

			static constexpr auto get_name = []() -> std::string { return "MODE"; };
			operator std::string() const override;
	};
}

#endif
