#ifndef PINGPONG_MESSAGES_MESSAGE_H_
#define PINGPONG_MESSAGES_MESSAGE_H_

#include <map>
#include <memory>
#include <string>

#include "core/server.h"
#include "messages/line.h"

namespace pingpong {
	class message;
	using message_ctor = std::function<message_ptr(pingpong::line)>;

	class message {
		public:
			pingpong::line line;

			message(const pingpong::line &line_): line(line_) {}

			static constexpr auto get_name = []() -> std::string { return "???"; };

			static std::map<std::string, message_ctor> ctors;
			virtual ~message() = 0;

			virtual operator std::string() const;

			/** Performs any actions to be taken when the message is received. For a PING, this would be replying with
			 *  a PONG; for most messages, it might just be dispatching the corresponding event. The function should
			 *  return false if a default message_event should be dispatched, or true if it should be suppressed. */
			virtual bool operator()(server_ptr) { return false; }

			server_ptr get_server() { return line.serv; }

			static message_ptr parse(const pingpong::line &);

			template <typename T>
			static void add_ctor() {
				message::ctors.insert({T::get_name(), [](pingpong::line line_) -> std::shared_ptr<T> {
					return std::make_shared<T>(line_);
				}});
			}

			template <typename T>
			bool is() {
				return bool(dynamic_cast<T *>(this));
			}
	};
}

#endif
