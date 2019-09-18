#ifndef PINGPONG_CORE_LOCAL_H_
#define PINGPONG_CORE_LOCAL_H_

#include <memory>
#include <string>

#include "core/channel.h"
#include "core/server.h"
#include "core/user.h"

namespace pingpong {
	/**
	 * For anything specific to one location (a channel or a user).
	 */
	class local {
		protected:
			local(const std::string &where_): where(where_) {}
			inline void check_destination() const;

		public:
			std::string where;

			bool is_user() const;
			bool is_channel() const;

			/** If the location is a user, this returns a pointer. Note that when the destination of a received message
			 *  is a user, that user shouldn't ever be anyone but you, unless you found a way to receive messages
			 *  intended for other users... */
			std::shared_ptr<user> get_user(server *) const;

			/** If the source of this message is a channel, this returns a pointer to the channel. */
			std::shared_ptr<channel> get_channel(server *) const;
	};
}

#endif
