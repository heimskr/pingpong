#ifndef PINGPONG_EVENTS_MODE_H_
#define PINGPONG_EVENTS_MODE_H_

#include "core/channel.h"
#include "core/local.h"
#include "core/modeset.h"
#include "core/user.h"

#include "events/event.h"

namespace pingpong {
	class mode_event: public server_event, public local {
		public:
			modeset mset;

			mode_event(server *serv, const std::string &where_, const modeset &mset_):
				server_event(serv), local(where_), mset(mset_) {}

			template <typename T>
			mode_event(const T &where_, const modeset &mset_):
				server_event(where_->serv), local(where_), mset(mset_) {}
	};
}

#endif
