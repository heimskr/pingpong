#ifndef PINGPONG_CORE_MODESET_H_
#define PINGPONG_CORE_MODESET_H_

#include <stdexcept>
#include <string>
#include <unordered_set>

namespace PingPong {
	class ModeSet {
		public:
			enum class ModeType {Self, Channel};
			
			/** There are two types of mode changes: user modes and channel modes. This indicates which type applies. */
			ModeType type = ModeType::Self;

			/** The raw string of mode changes, such as "-v+o". */
			std::string modes;

			/** A string of extra data for things like bans. */
			std::string extra;

			/** A set of all modes indicated as added by the message. */
			std::unordered_set<char> added {};

			/** A set of all modes indicated as removed by the message. */
			std::unordered_set<char> removed {};

			ModeSet() = default;

			/** Throws std::invalid_argument if the line is malformed. */
			ModeSet(ModeType type_, const std::string &modes_, const std::string &extra_ = "") noexcept(false);

			/** Returns whether the type is one of the defined types. This should never return false. */
			bool isTypeValid() const noexcept;

			/** Returns a string indicating the added and removed modes in the standard notation (e.g., "-S+n"), without
			 *  extra data like masks (for bans). */
			std::string modeString() const;

			/** Returns the modestring with any extra data appended. */
			operator std::string() const;
	};
}

#endif
