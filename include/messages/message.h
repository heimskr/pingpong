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
		protected:
			pingpong::line line;

		public:
			message(const pingpong::line &line_): line(line_) {}

			static constexpr auto get_name = []() -> std::string { return "???"; };

			static std::map<std::string, message_ctor> ctors;
			virtual ~message() = 0;

			virtual operator std::string() const;
			virtual void operator()(server_ptr) const {}

			static message_ptr parse(const pingpong::line &);
			virtual std::string name() const = 0;


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
