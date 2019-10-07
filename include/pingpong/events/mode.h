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
			pingpong::line line;

			mode_event(server *serv, const std::string &where_, const std::shared_ptr<pingpong::user> &who_,
			const modeset &mset_, const pingpong::line &line_ = {}):
				server_event(serv), local(where_), mset(mset_), who(who_), line(line_) {}

			template <typename T>
			mode_event(const T &where_, const std::shared_ptr<pingpong::user> &who_, const modeset &mset_,
			const pingpong::line &line_):
				server_event(where_->serv), local(where_), mset(mset_), who(who_), line(line_) {}

			std::string get_name() const {
				return who? who->name : (line? line.source.nick : "");
			}
	};
}

#endif
