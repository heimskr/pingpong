#ifndef PINGPONG_MESSAGES_MESSAGE_H_
#define PINGPONG_MESSAGES_MESSAGE_H_

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "pingpong/messages/line.h"

namespace pingpong {
	class message;
	class server;

	using message_ctor = std::function<std::shared_ptr<message>(pingpong::line)>;

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
			virtual bool operator()(server *) { return false; }

			server * get_server() { return line.serv; }

			static std::shared_ptr<message> parse(const pingpong::line &);

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

	class bad_message: public std::exception {
		public:
			const line original;

			bad_message(const line &original_): original(original_) {}

			const char * what() const noexcept {
				return "Malformed message";
			}
	};
}

#endif
