#include "core/modeset.h"

#include "lib/ansi.h"

namespace pingpong {
	void modeset::process() noexcept(false) {
		if (modes.empty())
			throw std::invalid_argument("Mode string is empty in modeset");

		// The first character has to be a plus or a minus.
		const char front = modes.front();
		if (front != '-' && front != '+')
			throw std::invalid_argument("Mode string doesn't start with a plus or minus in modeset");

		// Next, check the string to see whether it never has a plus/minus immediately following another plus/minus
		// and contains nothing other than pluses/minuses and letters (uppercase or lowercase).
		
		bool was_pm = false; // Whether the last character was a plus or a minus.
		char last_mode = '?';
		for (char c: modes) {
			// Ignore any spaces that might find their way into the string.
			if (c == ' ')
				continue;

			if (c == '+' || c == '-') {
				if (was_pm) // Can't have multiple pluses/minuses in a row.
					throw std::invalid_argument("Adjacent pluses/minuses in modeset");
				was_pm = true;
				last_mode = c;
				continue;
			} else if (c < 'A' || 'z' < c || ('Z' < c && c < 'a')) {
				// Letters only, please.
				throw std::invalid_argument("Invalid character encountered in modeset");
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
				throw std::invalid_argument("Unknown problem with modeset");
			}
		}

		// The last character can't be a plus or a minus; it has to be a letter.
		if (was_pm)
			throw std::invalid_argument("Last character in modeset was a plus or minus");
	}

	bool modeset::is_type_valid() const noexcept {
		return type == mode_type::self || type == mode_type::channel;
	}

	modeset::operator std::string() const {
		std::string out {"+"};
		out.reserve(2 + added.size() + removed.size());
		out.insert(out.end(), added.begin(), added.end());
		out.push_back('-');
		out.insert(out.end(), removed.begin(), removed.end());
		return out;
	}
}
