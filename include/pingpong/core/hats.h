#ifndef PINGPONG_CORE_HATS_H_
#define PINGPONG_CORE_HATS_H_

#include <ostream>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "pingpong/core/Defs.h"

namespace PingPong {
	/**
	 * Represents a collection of IRC hats.
	 */
	class HatSet {
		public:
			static const std::string order, reverseOrder;
			static std::unordered_map<Hat, int> ranks;
			static std::unordered_map<char, Hat> map;

		private:
			std::unordered_set<Hat> set;

			static std::unordered_set<Hat> allHats;

		public:
			HatSet(): set({}) {}
			HatSet(Hat hat): set({hat}) {}
			HatSet(const std::unordered_set<Hat> &set_): set(set_) {}
			HatSet(const std::string &);

			/** Returns the highest ranked hat in the set. */
			Hat highest() const;

			/** Returns the lowest ranked hat in the set. */
			Hat lowest() const;

			/** Returns the number of hats in the set (not including the "none" pseudo-hat). */
			size_t size() const;

			/** Returns whether the hat set contains any hats (not including the "none" pseudo-hat). */
			operator bool() const;

			/** Returns a string indicating all hats in the set, ordered from highest rank to lowest rank. */
			operator std::string() const;

			/** Returns whether the hat set's contents are the same as another's. */
			bool operator==(const HatSet &) const;

			/** Returns whether the hat set's contents differs from another's. */
			bool operator!=(const HatSet &) const;

			/** Returns whether this hat set outranks another. Compares only the highest hat in each set. */
			bool operator>(const HatSet &) const;

			/** Returns whether this hat set is outranked by another. Compares only the highest hat in each set. */
			bool operator<(const HatSet &) const;

			/** Returns whether the hat set contains a given hat (and no other hats). */
			bool operator==(Hat) const;

			/** Returns whether the hat set contains a given hat (and no other hats). */
			bool operator==(char) const;

			/** Returns whether the hat set contains no hats, multiple hats or a hat other than a given hat. */
			bool operator!=(Hat) const;

			/** Returns whether the hat set contains no hats, multiple hats or a hat other than a given hat. */
			bool operator!=(char) const;

			/** Returns whether the hat set contains a given hat (possibly in addition to other hats.) */
			bool operator%(Hat) const;

			/** Returns whether the hat set contains a given hat (possibly in addition to other hats.) */
			bool operator%(char) const;

			/** Returns a new hat set consisting of the shared hats of this hat set and another. */
			HatSet operator&(const HatSet &) const;

			/** Returns a new hat set containing a given hat if this hat set contains it, or an empty set otherwise. */
			HatSet operator&(Hat) const;

			/** Replaces the hat set's contents with a single hat. */
			HatSet & operator=(Hat);

			/** Replaces the hat set's contents with a single hat. */
			HatSet & operator=(char);

			/** Adds a hat to the hat set. */
			HatSet & operator+=(Hat);

			/** Adds a hat to the hat set. */
			HatSet & operator+=(char);

			/** Removes a hat from the hat set. */
			HatSet & operator-=(Hat);

			/** Removes a hat from the hat set. */
			HatSet & operator-=(char);

			/** Returns whether a character is a valid hat representation. */
			static bool isHat(char);

			/** Converts a character into a hat. Throws an exception if the character doesn't represent a hat. */
			static Hat getHat(char);

			/** Converts a string into a hat. Throws an exception if the string doesn't represent a single hat. */
			static Hat getHat(const std::string &);

			/** Separates a string like "@+pingpong" into a HatSet and a string for the nick. */
			static std::pair<HatSet, std::string> separate(const std::string &);

			friend std::ostream & operator<<(std::ostream &, const HatSet &);
	};

	std::ostream & operator<<(std::ostream &, const HatSet &);
	bool operator<(Hat, Hat);
	bool operator>(Hat, Hat);
}

#endif
