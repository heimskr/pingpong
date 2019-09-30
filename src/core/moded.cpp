#include <set>

#include "pingpong/core/moded.h"

namespace pingpong {
	size_t moded::apply_modes(const std::unordered_set<char> remove, const std::unordered_set<char> add) {
		size_t changes = 0;

		for (char c: remove)
			changes += modes.erase(c);

		for (char c: add) {
			if (modes.count(c) == 0) {
				++changes;
				modes.insert(c);
			}
		}
		
		return changes;
	}

	std::string moded::mode_str() const {
		std::set<char> sorted {modes.begin(), modes.end()};
		std::string out {"+"};
		out.reserve(sorted.size() + 1);
		for (char c: sorted)
			out.push_back(c);
		return out;
	}
}
