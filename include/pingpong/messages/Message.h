#ifndef PINGPONG_MESSAGES_MESSAGE_H_
#define PINGPONG_MESSAGES_MESSAGE_H_

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "pingpong/messages/Line.h"

namespace PingPong {
	class Message;
	class Server;

	using Message_ctor = std::function<std::shared_ptr<Message>(PingPong::Line)>;

	class Message {
		public:
			PingPong::Line line;

			Message(const PingPong::Line &line_): line(line_) {}

			static constexpr auto getName = []() -> std::string { return "???"; };

			static std::map<std::string, Message_ctor> ctors;
			virtual ~Message() = 0;

			virtual operator std::string() const;

			/** Performs any actions to be taken when the message is received. For a PING, this would be replying with
			 *  a PONG; for most messages, it might just be dispatching the corresponding event. The function should
			 *  return false if a default MessageEvent should be dispatched, or true if it should be suppressed. */
			virtual bool operator()(Server *) { return false; }

			Server * getServer() { return line.server; }

			static std::shared_ptr<Message> parse(const PingPong::Line &);

			template <typename T>
			static void addConstructor() {
				Message::ctors.insert({T::getName(), [](PingPong::Line line_) -> std::shared_ptr<T> {
					return std::make_shared<T>(line_);
				}});
			}

			template <typename T>
			bool is() {
				return bool(dynamic_cast<T *>(this));
			}
	};

	class BadMessage: public std::exception {
		public:
			const Line original;

			BadMessage(const Line &original_): original(original_) {}

			const char * what() const noexcept {
				return "Malformed message";
			}
	};
}

#endif
