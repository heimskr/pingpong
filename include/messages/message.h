#ifndef MESSAGES_MESSAGE_H_
#define MESSAGES_MESSAGE_H_

#include <map>
#include <memory>
#include <string>

#include "line.h"

namespace pingpong {
	class message;
	using message_ptr = std::unique_ptr<pingpong::message>;
	using message_ctor = std::function<std::unique_ptr<message>(pingpong::line)>;

	class message {
		protected:
			line line;

		public:
			message(pingpong::line line_): line(line_) {}
			static constexpr auto get_name = []() -> std::string { return "???"; };

			static std::map<std::string, message_ctor> ctors;
			virtual ~message() = 0;

			virtual operator std::string() const;

			static message_ptr parse(const std::string &);
			static message_ptr parse(const pingpong::line &);

			template <class T>
			static void add_ctor() {
				message::ctors.insert({T::get_name(), [](pingpong::line line_) -> std::unique_ptr<T> {
					return std::unique_ptr<T>(new T(line_));
				}});
			}

			template <class T>
			static bool is(message_ptr &ptr) {
				return bool(dynamic_cast<T *>(&*ptr));
			}
	};
}

#endif
