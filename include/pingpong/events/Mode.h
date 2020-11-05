#ifndef PINGPONG_EVENTS_MODE_H_
#define PINGPONG_EVENTS_MODE_H_

#include "pingpong/core/Channel.h"
#include "pingpong/core/Local.h"
#include "pingpong/core/ModeSet.h"
#include "pingpong/core/User.h"

#include "pingpong/events/Event.h"

namespace PingPong {
	class ModeEvent: public ServerEvent, public Local {
		public:
			ModeSet modeSet;
			std::shared_ptr<PingPong::User> who;
			PingPong::Line line;

			ModeEvent(Server *server_, const std::string &where_, const std::shared_ptr<PingPong::User> &who_,
			const ModeSet &modeset, const PingPong::Line &line_ = {}):
				ServerEvent(server_), Local(where_), modeSet(modeset), who(who_), line(line_) {}

			template <typename T>
			ModeEvent(const T &where_, const std::shared_ptr<PingPong::User> &who_, const ModeSet &modeset,
			const PingPong::Line &line_):
				ServerEvent(where_->server), Local(where_), modeSet(modeset), who(who_), line(line_) {}

			std::string getName() const {
				return who? who->name : (line? line.source.nick : "");
			}
	};
}

#endif
