#include <string>

#include "pingpong/core/Hats.h"

namespace PingPong {
	std::unordered_set<Hat> HatSet::allHats = {Hat::Owner, Hat::Admin, Hat::Op, Hat::Halfop, Hat::Voiced, Hat::None};

	HatSet::HatSet(const std::string &str): set({}) {
		for (char ch: str) {
			if (!isHat(ch))
				throw std::invalid_argument("Invalid character in hat string");
			set.insert(getHat(ch));
		}
	}

	Hat HatSet::highest() const {
		for (char ch: order) {
			if (*this % ch)
				return static_cast<Hat>(ch);
		}

		return Hat::None;
	}

	Hat HatSet::lowest() const {
		if (*this == Hat::None)
			return Hat::None;

		for (char ch: reverseOrder) {
			if (*this % ch)
				return static_cast<Hat>(ch);
		}

		return Hat::None;
	}

	size_t HatSet::size() const {
		size_t out = 0;
		for (Hat h: set) {
			if (h != Hat::None)
				++out;
		}
		return out;
	}

	HatSet::operator bool() const {
		return *this != Hat::None;
	}

	HatSet::operator std::string() const {
		if (*this == Hat::None)
			return "";
		std::string out;
		out.reserve(set.size());
		for (char ch: order) {
			if (ch != ' ' && set.count(static_cast<Hat>(ch)) != 0)
				out += ch;
		}
		return out;
	}


	bool HatSet::operator==(const HatSet &other) const {
		return set == other.set;
	}

	bool HatSet::operator!=(const HatSet &other) const {
		return !(*this == other);
	}

	bool HatSet::operator>(const HatSet &other) const {
		return highest() > other.highest();
	}

	bool HatSet::operator<(const HatSet &other) const {
		return highest() < other.highest();
	}

	bool HatSet::operator==(Hat h) const {
		return (h == Hat::None && set.empty()) || (set.size() == 1 && set.count(h) == 1);
	}

	bool HatSet::operator==(char ch) const {
		return *this == getHat(ch);
	}

	bool HatSet::operator!=(Hat h) const {
		return !(*this == h);
	}

	bool HatSet::operator!=(char ch) const {
		return !(*this == ch);
	}


	bool HatSet::operator%(Hat h) const {
		return set.count(h) == 1;
	}

	bool HatSet::operator%(char ch) const {
		return *this % getHat(ch);
	}


	HatSet HatSet::operator&(const HatSet &other) const {
		HatSet out {};
		for (Hat h: allHats) {
			if (*this % h && other % h)
				out += h;
		}

		return out;
	}

	HatSet HatSet::operator&(Hat h) const {
		return (*this % h)? HatSet(h) : HatSet();
	}


	HatSet & HatSet::operator=(Hat hat) {
		if (hat == Hat::None)
			set.clear();
		else
			set = {hat};
		return *this;
	}

	HatSet & HatSet::operator=(char ch) {
		set = {getHat(ch)};
		return *this;
	}


	HatSet & HatSet::operator+=(Hat hat) {
		if (hat == Hat::None)
			set.clear();
		else
			set.insert(hat);
		return *this;
	}

	HatSet & HatSet::operator+=(char ch) {
		return *this += getHat(ch);
	}


	HatSet & HatSet::operator-=(Hat hat) {
		if (hat != Hat::None)
			set.erase(hat);
		return *this;
	}

	HatSet & HatSet::operator-=(char ch) {
		return *this -= getHat(ch);
	}


	bool HatSet::isHat(char ch) {
		return allHats.count(static_cast<Hat>(ch)) != 0;
	}

	Hat HatSet::getHat(char ch) {
		if (HatSet::order.find(ch) != std::string::npos)
			return static_cast<Hat>(ch);
		if (HatSet::map.count(ch) != 0)
			return HatSet::map.at(ch);
		throw std::invalid_argument("Invalid hat: '" + std::string(1, ch) + "'");
	}

	Hat HatSet::getHat(const std::string &str) {
		return str.empty() || str[0] == ' '? Hat::None : getHat(str[0]);
	}

	std::pair<HatSet, std::string> HatSet::separate(const std::string &str) {
		const size_t nickpos = str.find_first_not_of("~&@%+");
		if (nickpos == 0)
			return {{}, str};
		return {str.substr(0, nickpos), str.substr(nickpos)};
	}
	
	const std::string HatSet::order = "~&@%+ ";
	const std::string HatSet::reverseOrder = " +%@&~";

	std::unordered_map<char, Hat> HatSet::map {
		{'q', Hat::Owner},
		{'a', Hat::Admin},
		{'o', Hat::Op},
		{'h', Hat::Halfop},
		{'v', Hat::Voiced},
	};

	std::unordered_map<Hat, int> HatSet::ranks {
		{Hat::Owner,  5},
		{Hat::Admin,  4},
		{Hat::Op,     3},
		{Hat::Halfop, 2},
		{Hat::Voiced, 1},
		{Hat::None,   0}
	};

	std::ostream & operator<<(std::ostream &os, const HatSet &set) {
		return os << std::string(set);
	}

	bool operator<(Hat left, Hat right) {
		return HatSet::ranks.at(left) < HatSet::ranks.at(right);
	}

	bool operator>(Hat left, Hat right) {
		return HatSet::ranks.at(left) > HatSet::ranks.at(right);
	}
}
