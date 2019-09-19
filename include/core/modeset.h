#ifndef PINGPONG_CORE_MODESET_H_
#define PINGPONG_CORE_MODESET_H_

#include <stdexcept>
#include <string>
#include <unordered_set>

namespace pingpong {
	class modeset {
		public:
			enum class mode_type {self, channel};
			
			/** There are two types of mode changes: user modes and channel modes. This indicates which type applies. */
			mode_type type {mode_type::self};

			/** The raw string of mode changes, such as "-v+o". */
			std::string modes;

			/** A string of extra data for things like bans. */
			std::string extra;

			/** A set of all modes indicated as added by the message. */
			std::unordered_set<char> added {};

			/** A set of all modes indicated as removed by the message. */
			std::unordered_set<char> removed {};

			modeset() = default;

			modeset(mode_type type_, const std::string &modes_, const std::string &extra_ = ""):
				type(type_), modes(modes_), extra(extra_) {}

			/** Processes the modes string: fills in the added/removed sets. Throws std::invalid_argument if the line is
			 *  malformed. */
			void process() noexcept(false);

			/** Returns whether the type is one of the defined types. This should never return false. */
			bool is_type_valid() const noexcept;

			/** Returns a string indicating the added and removed modes in the standard notation (e.g., "-S+n"). */
			operator std::string() const;
	};
}

#endif
