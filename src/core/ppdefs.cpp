#include "pingpong/core/defs.h"

namespace pingpong {
	std::unordered_map<char, hat> hat_map {
		{'q', hat::owner},
		{'a', hat::admin},
		{'o', hat::op},
		{'h', hat::halfop},
		{'v', hat::voiced},
	};

	std::unordered_map<hat, int> hat_ranks {
		{hat::owner,  5},
		{hat::admin,  4},
		{hat::op,     3},
		{hat::halfop, 2},
		{hat::voiced, 1},
		{hat::none,   0}
	};

	bool operator<(hat left, hat right) {
		return hat_ranks.at(left) < hat_ranks.at(right);
	}

	bool operator>(hat left, hat right) {
		return hat_ranks.at(left) > hat_ranks.at(right);
	}
}
