#ifndef PINGPONG_CORE_HATS_H_
#define PINGPONG_CORE_HATS_H_

#include <ostream>
#include <unordered_set>
#include <unordered_map>

#include "pingpong/core/defs.h"

namespace pingpong {
	/**
	 * Represents a collection of IRC hats.
	 */
	class hat_set {
		public:
			static const std::string order, reverse_order;
			static std::unordered_map<hat, int> ranks;
			static std::unordered_map<char, hat> map;

		private:
			std::unordered_set<hat> set;

			static std::unordered_set<hat> all_hats;

		public:
			hat_set(): set({}) {}
			hat_set(hat hat): set({hat}) {}
			hat_set(const std::unordered_set<hat> &set_): set(set_) {}

			/** Returns the highest ranked hat in the set. */
			hat highest() const;

			/** Returns the lowest ranked hat in the set. */
			hat lowest() const;

			/** Returns the number of hats in the set (not including the "none" pseudo-hat). */
			size_t size() const;

			/** Returns a string indicating all hats in the set, ordered from highest rank to lowest rank. */
			operator std::string() const;

			/** Returns whether the hat set's contents are the same as another's. */
			bool operator==(const hat_set &) const;

			/** Returns whether the hat set contains a given hat (and no other hats). */
			bool operator==(hat) const;

			/** Returns whether the hat set contains a given hat (and no other hats). */
			bool operator==(char) const;

			/** Returns whether the hat set's contents differs from another's. */
			bool operator!=(const hat_set &) const;

			/** Returns whether the hat set contains no hats, multiple hats or a hat other than a given hat. */
			bool operator!=(hat) const;

			/** Returns whether the hat set contains no hats, multiple hats or a hat other than a given hat. */
			bool operator!=(char) const;

			/** Returns whether the hat set contains a given hat (possibly in addition to other hats.) */
			bool operator%(hat) const;

			/** Returns whether the hat set contains a given hat (possibly in addition to other hats.) */
			bool operator%(char) const;

			/** Returns a new hat set consisting of the shared hats of this hat set and another. */
			hat_set operator&(const hat_set &) const;

			/** Returns a new hat set containing a given hat if this hat set contains it, or an empty set otherwise. */
			hat_set operator&(hat) const;

			/** Replaces the hat set's contents with a single hat. */
			hat_set & operator=(hat);

			/** Replaces the hat set's contents with a single hat. */
			hat_set & operator=(char);

			/** Adds a hat to the hat set. */
			hat_set & operator+=(hat);

			/** Adds a hat to the hat set. */
			hat_set & operator+=(char);

			/** Removes a hat from the hat set. */
			hat_set & operator-=(hat);

			/** Removes a hat from the hat set. */
			hat_set & operator-=(char);

			/** Returns whether a character is a valid hat representation. */
			static bool is_hat(char);

			/** Converts a character into a hat. Throws an exception if the character doesn't represent a hat. */
			static hat get_hat(char);

			/** Converts a string into a hat. Throws an exception if the string doesn't represent a single hat. */
			static hat get_hat(const std::string &);

			friend std::ostream & operator<<(std::ostream &, const hat_set &);
	};

	bool operator<(hat, hat);
	bool operator>(hat, hat);
}

#endif
