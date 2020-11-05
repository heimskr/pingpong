#ifndef PINGPONG_EVENTS_ERROR_H_
#define PINGPONG_EVENTS_ERROR_H_

#include <memory>
#include <string>

#include "pingpong/core/Server.h"
#include "pingpong/events/Event.h"
#include "pingpong/messages/Message.h"

namespace PingPong {
	struct ErrorEvent: public Event {
		std::string message;
		// If true, the error will be displayed in the current window rather than the status window.
		bool currentWindow;

		ErrorEvent(const std::string &message_, bool current_window = true):
			message(message_), currentWindow(current_window) {}
	};
}

#endif
