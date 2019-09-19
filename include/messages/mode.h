#ifndef PINGPONG_MESSAGES_MODE_H_
#define PINGPONG_MESSAGES_MODE_H_

#include <unordered_set>

#include "core/modeset.h"
#include "messages/message.h"

namespace pingpong {
	class mode_message: public message {
		private:
			/** Validates the string of additions/removals with a touch of paranoia. */
			void validate_modes(const std::string &, const pingpong::line &);

		public:
			modeset mset;

			/** If the mode change is for a channel, this stores a pointer to the channel. */
			std::shared_ptr<pingpong::channel> chan;

			mode_message(const pingpong::line &line_);

			/** Applies the changes to the relevant channel (for channel mode changes) or to you (for user mode
			 *  changes). Returns true if the changes were successfully applied. */
			bool apply();

			static constexpr auto get_name = []() -> std::string { return "MODE"; };
			operator std::string() const override;
	};
}

#endif
