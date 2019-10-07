#include <string>

#include "pingpong/core/hats.h"

#include "lib/formicine/ansi.h"

namespace pingpong {
	std::unordered_set<hat> hat_set::all_hats = {hat::owner, hat::admin, hat::op, hat::halfop, hat::voiced, hat::none};

	hat hat_set::highest() const {
		for (char ch: order) {
			if (*this % ch)
				return static_cast<hat>(ch);
		}

		return hat::none;
	}

	hat hat_set::lowest() const {
		if (*this == hat::none)
			return hat::none;

		for (char ch: reverse_order) {
			if (*this % ch)
				return static_cast<hat>(ch);
		}

		return hat::none;
	}

	hat_set::operator std::string() const {
		if (*this == hat::none)
			return "";
		std::string out;
		out.reserve(set.size());
		for (char ch: order) {
			if (set.count(static_cast<hat>(ch)) != 0)
				out += ch;
		}
		return out;
	}


	bool hat_set::operator==(const hat_set &other) const {
		return set == other.set;
	}

	bool hat_set::operator==(hat h) const {
		return (h == hat::none && set.empty()) || (set.size() == 1 && set.count(h) == 1);
	}

	bool hat_set::operator==(char ch) const {
		return *this == get_hat(ch);
	}


	bool hat_set::operator!=(const hat_set &other) const {
		return !(*this == other);
	}

	bool hat_set::operator!=(hat h) const {
		return !(*this == h);
	}

	bool hat_set::operator!=(char ch) const {
		return !(*this == ch);
	}


	bool hat_set::operator%(hat h) const {
		return set.count(h) == 1;
	}

	bool hat_set::operator%(char ch) const {
		return *this % get_hat(ch);
	}


	hat_set hat_set::operator&(const hat_set &other) const {
		hat_set out {};
		for (hat h: all_hats) {
			if (*this % h && other % h)
				out += h;
		}

		return out;
	}

	hat_set hat_set::operator&(hat h) const {
		return *this % h? hat_set(h) : hat_set();
	}


	hat_set & hat_set::operator=(hat hat) {
		set = {hat};
		return *this;
	}

	hat_set & hat_set::operator=(char ch) {
		set = {get_hat(ch)};
		return *this;
	}


	hat_set & hat_set::operator+=(hat hat) {
		if (hat == hat::none)
			set.clear();
		else
			set.insert(hat);
		return *this;
	}

	hat_set & hat_set::operator+=(char ch) {
		return *this += get_hat(ch);
	}


	hat_set & hat_set::operator-=(hat hat) {
		if (hat != hat::none)
			set.erase(hat);
		return *this;
	}

	hat_set & hat_set::operator-=(char ch) {
		return *this -= get_hat(ch);
	}


	hat hat_set::get_hat(char ch) {
		if (hat_set::order.find(ch) != std::string::npos)
			return static_cast<hat>(ch);
		if (hat_set::map.count(ch) != 0)
			return hat_set::map.at(ch);
		throw std::invalid_argument("Invalid hat: '" + std::string(1, ch) + "'");
	}

	hat hat_set::get_hat(const std::string &str) {
		return str.empty() || str[0] == ' '? hat::none : get_hat(str[0]);
	}
	

	const std::string hat_set::order = "~&@%+ ";
	const std::string hat_set::reverse_order = " +%@&~";

	std::unordered_map<char, hat> hat_set::map {
		{'q', hat::owner},
		{'a', hat::admin},
		{'o', hat::op},
		{'h', hat::halfop},
		{'v', hat::voiced},
	};

	std::unordered_map<hat, int> hat_set::ranks {
		{hat::owner,  5},
		{hat::admin,  4},
		{hat::op,     3},
		{hat::halfop, 2},
		{hat::voiced, 1},
		{hat::none,   0}
	};

	std::ostream & operator<<(std::ostream &os, const hat_set &set) {
		return os << std::string(set);
	}

	bool operator<(hat left, hat right) {
		return hat_set::ranks.at(left) < hat_set::ranks.at(right);
	}

	bool operator>(hat left, hat right) {
		return hat_set::ranks.at(left) > hat_set::ranks.at(right);
	}
}
