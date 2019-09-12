#ifndef PINGPONG_EVENTS_EVENT_H_
#define PINGPONG_EVENTS_EVENT_H_

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "core/debug.h"
#include "core/server.h"
#include "core/user.h"
#include "core/pputil.h"

namespace pingpong {
	class event {
		private:

		protected:
			bool is_empty = false;
			event(bool empty): is_empty(empty) {}

		public:
			long stamp = util::timestamp();

			virtual ~event() = default;
			event(): is_empty(true) {}

			operator bool() const { return !is_empty; }
	};

	using listener_fn = std::function<void(event *)>;

	class fake_event: public event { public: fake_event(): event() {} };

	class events {
		private:
			static std::multimap<std::string, listener_fn> listeners;

		public:
			template <typename T>
			static void listen(std::function<void(T *)> fn) {
				listeners.insert(std::pair<std::string, listener_fn>(
					std::string(typeid(T).name()),
					[=](event *ev) {
						fn(dynamic_cast<T *>(ev));
					}
				));
			}

			template <typename T>
			static void dispatch(T *ptr) {
				auto range = listeners.equal_range(typeid(T).name());
				for (auto i = range.first; i != range.second; ++i)
					i->second(ptr);
			}

			template <typename T>
			static void dispatch(std::unique_ptr<T> &&ptr) {
				dispatch(ptr.get());
			}

			template <typename T, typename... Args>
			static void dispatch(Args && ...args) {
				dispatch<T>(std::make_unique<T>(std::forward<Args>(args)...));
			}
	};

	// For events local to one server.
	class server_event: public event {
		public:
			server_ptr serv;

			server_event(server_ptr serv_): event(false), serv(serv_) {}
	};

	// For events local to one channel on one server.
	class channel_event: public server_event {
		public:
			channel_ptr chan;

			channel_event(channel_ptr chan_, server_ptr serv_): server_event(serv_), chan(chan_) {
				chan->serv = serv;
			}

			channel_event(channel_ptr chan_): server_event(chan_->serv), chan(chan_) {
				if (!chan_->has_server())
					throw std::invalid_argument("Channel is not associated with a server");
			}
	};

	// For events local to one user in one channel on one server.
	class user_event: public channel_event {
		public:
			user_ptr who;
			std::string content;

			user_event(user_ptr, channel_ptr, const std::string & = "");
	};
}

#endif
