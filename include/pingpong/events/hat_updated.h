#ifndef PINGPONG_EVENTS_HAT_UPDATED_H_
#define PINGPONG_EVENTS_HAT_UPDATED_H_

#include "pingpong/core/defs.h"
#include "pingpong/events/event.h"

namespace pingpong {
	struct hat_updated_event: public user_event {
		hat old_hat, new_hat;
		
		hat_updated_event(const std::shared_ptr<user> &who_, const std::shared_ptr<channel> &chan_, hat old_hat_,
		hat new_hat_):
			user_event(who_, chan_, std::string(1, static_cast<char>(new_hat_))), old_hat(old_hat_), new_hat(new_hat_) {
			}
	};
}

#endif
