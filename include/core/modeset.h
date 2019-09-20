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

			/** Throws std::invalid_argument if the line is malformed. */
			modeset(mode_type type_, const std::string &modes_, const std::string &extra_ = "") noexcept(false);

			/** Returns whether the type is one of the defined types. This should never return false. */
			bool is_type_valid() const noexcept;

			/** Returns a string indicating the added and removed modes in the standard notation (e.g., "-S+n"), without
			 *  extra data like masks (for bans). */
			std::string mode_str() const;

			/** Returns the modestring with any extra data appended. */
			operator std::string() const;
	};
}

#endif
