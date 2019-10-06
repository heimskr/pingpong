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
			static const std::string order;

			static std::unordered_map<hat, int> ranks;
			static std::unordered_map<char, hat> map;

		private:
			static std::unordered_set<hat> all_hats;
			std::unordered_set<hat> set;

		public:
			hat_set(): set({hat::none}) {}
			hat_set(hat hat): set({hat}) {}
			hat_set(const std::unordered_set<hat> &set_): set(set_) {}

			operator std::string() const;

			bool operator==(hat) const;
			bool operator==(char) const;

			bool operator%(hat) const;
			bool operator%(char) const;

			hat_set operator&(const hat_set &) const;
			hat_set operator&(hat) const;

			hat_set & operator=(hat);
			hat_set & operator=(char);

			hat_set & operator+=(hat);
			hat_set & operator+=(char);

			hat_set & operator-=(hat);
			hat_set & operator-=(char);

			static hat get_hat(char);
			static hat get_hat(const std::string &);

			friend std::ostream & operator<<(std::ostream &, const hat_set &);
	};

	bool operator<(hat, hat);
	bool operator>(hat, hat);
}

#endif
