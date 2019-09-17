#ifndef PINGPONG_EVENTS_EVENT_H_
#define PINGPONG_EVENTS_EVENT_H_

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "core/channel.h"
#include "core/debug.h"
#include "core/server.h"
#include "core/user.h"
#include "core/pputil.h"

namespace pingpong {
	class event {
		private:

		protected:
			bool is_empty = false;
			event(bool empty, const std::string &content_): is_empty(empty), content(content_) {}

		public:
			long stamp = util::timestamp();
			std::string content;

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
			server *serv;

			server_event(server *serv_, const std::string &content_ = ""): event(false, content_), serv(serv_) {}
	};

	// For events local to one channel on one server, such as topic changes.
	class channel_event: public server_event {
		public:
			std::shared_ptr<channel> chan;

			channel_event(std::shared_ptr<channel>, server *, const std::string & = "");
			channel_event(std::shared_ptr<channel>, const std::string & = "");
	};

	// For events local to one user in one channel on one server, such as joins and privmsgs.
	// This can also be used for things like quits, which are specific to a user and server but not to a channel, by
	// leaving the channel pointer null.
	class user_event: public channel_event {
		public:
			std::shared_ptr<user> who;

			user_event(std::shared_ptr<user>, std::shared_ptr<channel>, const std::string & = "");
			user_event(std::shared_ptr<user> who_, server *serv_, const std::string &content_ = ""):
				user_event(who_, static_cast<std::shared_ptr<channel>>(nullptr), content_) { serv = serv_; }
	};

	// For events local to two users in one channel on one server, such as kicks.
	class targeted_event: public user_event {
		public:
			std::shared_ptr<user> whom;

			targeted_event(std::shared_ptr<user>, std::shared_ptr<user>, std::shared_ptr<channel>, const std::string & = "");
			targeted_event(std::shared_ptr<user> who_, std::shared_ptr<user> whom_, server *serv_, const std::string &content_ = ""):
				targeted_event(who_, whom_, static_cast<std::shared_ptr<channel>>(nullptr), content_) { serv = serv_; }
	};
}

#endif
