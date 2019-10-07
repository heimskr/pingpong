#ifndef PINGPONG_CORE_USER_H_
#define PINGPONG_CORE_USER_H_

#include <memory>
#include <set>
#include <string>
#include <unordered_set>

#include "pingpong/core/defs.h"
#include "pingpong/core/mask.h"
#include "pingpong/core/moded.h"
#include "pingpong/core/server.h"
#include "pingpong/core/util.h"

namespace pingpong {
	class user: public moded {
		public:
			std::string name, account_name = "";
			mask info {};

			pingpong::server *serv = nullptr;
			std::set<std::weak_ptr<channel>, weakptr_compare<channel>> channels = {};

			user(const std::string &name_, server *serv_): name(name_), serv(serv_) {}

			void rename(const std::string &);
			operator std::string() const { return name; }
			
			template <typename T>
			user & operator+=(T chan) {
				channels.insert(std::weak_ptr<channel>(chan));
				return *this;
			}

			friend std::ostream & operator<<(std::ostream &os, const user &who);

			bool is_self() const;

			bool operator==(const user &) const;
	};
}

#endif
