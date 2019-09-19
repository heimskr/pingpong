#ifndef PINGPONG_CORE_MODED_H_
#define PINGPONG_CORE_MODED_H_

#include <string>
#include <unordered_set>

namespace pingpong {
	/**
	 * Represents anything that has modes. This includes channels and users (though it's not possible to know anyone's
	 * modes but your own).
	 */
	class moded {
		public:
			std::unordered_set<char> modes;

			/** Applies modes changes to the mode set. Returns the number of changes that were made. The set of modes to
			 *  remove is applied first, followed by the set of modes to add. */
			size_t apply_modes(const std::unordered_set<char> remove, const std::unordered_set<char> add);

			/** Returns a string representing all the modes. */
			std::string mode_str() const;
	};
}

#endif
