#ifndef PINGPONG_EVENTS_ERROR_H_
#define PINGPONG_EVENTS_ERROR_H_

#include <memory>
#include <string>

#include "core/server.h"
#include "events/event.h"
#include "messages/message.h"

namespace pingpong {
	class error_event: public event {
		public:
			std::string message;
			// If true, the error will be displayed in the current window rather than the status window.
			bool current_window;

			error_event(const std::string &message_, bool current_window_ = true):
				message(message_), current_window(current_window_) {}
	};
}

#endif
