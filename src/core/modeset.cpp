#include "core/modeset.h"

namespace pingpong {
	

	bool modeset::process() {
		if (modes.empty())
			return false;

		// The first character has to be a plus or a minus.
		const char front = modes.front();
		if (front != '-' && front != '+')
			return false;

		// Next, check the string to see whether it never has a plus/minus immediately following another plus/minus
		// and contains nothing other than pluses/minuses and letters (uppercase or lowercase).
		
		bool was_pm = false; // Whether the last character was a plus or a minus.
		char last_mode = '?';
		for (char c: modes) {
			if (c == '+' || c == '-') {
				if (was_pm) // Can't have multiple pluses/minuses in a row.
					return false;
				was_pm = true;
				last_mode = c;
				continue;
			} else if (c < 'A' || 'z' < c || ('Z' < c && c < 'a')) {
				// Letters only, please.
				return false;
			}

			was_pm = false;

			// Now we add the characters to the removed/added sets and make sure no character is contained in both sets.
			if (last_mode == '+') {
				removed.erase(c);
				added.insert(c);
			} else if (last_mode == '-') {
				added.erase(c);
				removed.insert(c);
			} else {
				// Shouldn't ever happen.
				return false;
			}
		}

		// The last character can't be a plus or a minus; it has to be a letter.
		return !was_pm;
	}
}
