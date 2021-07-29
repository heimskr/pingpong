#include "pingpong/core/ModeSet.h"

#include "lib/formicine/ansi.h"

namespace PingPong {
	ModeSet::ModeSet(ModeType type_, const std::string &modes_, const std::string &extra_):
	type(type_), modes(modes_), extra(extra_) {
		if (modes.empty())
			throw std::invalid_argument("Mode string is empty in ModeSet");

		// It's possible to have a ModeSet with no additions or removals—it will be simply "+".
		if (modes == "+")
			return;

		// The first character has to be a plus or a minus.
		const char front = modes.front();
		if (front != '-' && front != '+')
			throw std::invalid_argument("Mode string doesn't start with a plus or minus in ModeSet");

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
					throw std::invalid_argument("Adjacent pluses/minuses in ModeSet");
				was_pm = true;
				last_mode = c;
				continue;
			} else if (c < 'A' || 'z' < c || ('Z' < c && c < 'a')) {
				// Letters only, please.
				throw std::invalid_argument("Invalid character encountered in ModeSet");
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
				throw std::invalid_argument("Unknown problem with ModeSet");
			}
		}

		// The last character can't be a plus or a minus; it has to be a letter.
		if (was_pm)
			throw std::invalid_argument("Last character in ModeSet was a plus or minus");
	}

	bool ModeSet::isTypeValid() const noexcept {
		return type == ModeType::Self || type == ModeType::Channel;
	}

	std::string ModeSet::modeString() const {
		std::string out;
		out.reserve(2 + added.size() + removed.size());

		if (!added.empty())
			out.push_back('+');
		out.insert(out.end(), added.begin(), added.end());

		if (!removed.empty())
			out.push_back('-');
		out.insert(out.end(), removed.begin(), removed.end());
		return out;
	}

	ModeSet::operator std::string() const {
		std::string out = modeString();

		if (!extra.empty()) {
			if (!out.empty())
				out.push_back(' ');
			out += extra;
		}

		return out;
	}

	size_t ModeSet::count() const {
		return added.size() + removed.size();
	}
}
