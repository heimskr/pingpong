#ifndef PINGPONG_EVENTS_HATS_UPDATED_H_
#define PINGPONG_EVENTS_HATS_UPDATED_H_

#include "pingpong/core/defs.h"
#include "pingpong/core/hats.h"

#include "pingpong/events/event.h"

namespace pingpong {
	struct hats_updated_event: public user_event {
		hat_set old_hats, new_hats;
		
		hats_updated_event(const std::shared_ptr<user> &who_, const std::shared_ptr<channel> &chan_,
		const hat_set &old_hats_, const hat_set &new_hats_):
			user_event(who_, chan_, std::string(1, static_cast<char>(new_hat_))),
			old_hats(old_hats_), new_hats(new_hats_) {}
	};
}

#endif
