#ifndef PINGPONG_EVENTS_MODE_H_
#define PINGPONG_EVENTS_MODE_H_

#include "pingpong/core/channel.h"
#include "pingpong/core/local.h"
#include "pingpong/core/modeset.h"
#include "pingpong/core/user.h"

#include "pingpong/events/event.h"

namespace pingpong {
	class mode_event: public server_event, public local {
		public:
			modeset mset;
			std::shared_ptr<pingpong::user> who;

			mode_event(server *serv, const std::string &where_, const std::shared_ptr<pingpong::user> &who_,
			const modeset &mset_):
				server_event(serv), local(where_), mset(mset_), who(who_) {}

			template <typename T>
			mode_event(const T &where_, const std::shared_ptr<pingpong::user> &who_, const modeset &mset_):
				server_event(where_->serv), local(where_), mset(mset_), who(who_) {}
	};
}

#endif