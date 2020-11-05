#ifndef PINGPONG_CORE_USER_H_
#define PINGPONG_CORE_USER_H_

#include <memory>
#include <set>
#include <string>
#include <unordered_set>

#include "pingpong/core/Defs.h"
#include "pingpong/core/Mask.h"
#include "pingpong/core/Moded.h"
#include "pingpong/core/Server.h"
#include "pingpong/core/Util.h"

namespace PingPong {
	class User: public Moded {
		public:
			std::string name;
			Mask info {};
			PingPong::Server *server = nullptr;
			std::set<std::weak_ptr<Channel>, weakptr_compare<Channel>> channels = {};

			std::string accountName = "";
			std::string serverName = "";
			std::string realname = "";
			long idleSince = -1, signonTime = -1;

			User(const std::string &name_, Server *server_): name(name_), server(server_) {}

			void rename(const std::string &);
			operator std::string() const { return name; }
			
			template <typename T>
			User & operator+=(T chan) {
				channels.insert(std::weak_ptr<Channel>(chan));
				return *this;
			}

			friend std::ostream & operator<<(std::ostream &os, const User &who);

			bool isSelf() const;

			bool operator==(const User &) const;
	};
}

#endif
