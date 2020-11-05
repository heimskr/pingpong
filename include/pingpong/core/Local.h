#ifndef PINGPONG_CORE_LOCAL_H_
#define PINGPONG_CORE_LOCAL_H_

#include <memory>
#include <string>

#include "pingpong/core/Channel.h"
#include "pingpong/core/Server.h"
#include "pingpong/core/User.h"

namespace PingPong {
	/**
	 * For anything specific to one location (a channel or a user).
	 */
	class Local {
		protected:
			Local(const std::string &where_): where(where_) {}
			Local(std::shared_ptr<User> user_): Local(user_->name) {}
			Local(std::shared_ptr<Channel> chan_): Local(chan_->name) {}

			inline void checkDestination() const;

		public:
			std::string where;

			bool isUser() const;
			bool isChannel() const;

			/** If the location is a user, this returns a pointer. Note that when the destination of a received message
			 *  is a user, that user shouldn't ever be anyone but you, unless you found a way to receive messages
			 *  intended for other users... */
			std::shared_ptr<User> getUser(Server *, bool update_case = false) const;

			/** If the source of this message is a channel, this returns a pointer to the channel. */
			std::shared_ptr<Channel> getChannel(Server *) const;
	};
}

#endif
