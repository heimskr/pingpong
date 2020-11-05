#ifndef PINGPONG_EVENTS_EVENT_H_
#define PINGPONG_EVENTS_EVENT_H_

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "pingpong/core/Channel.h"
#include "pingpong/core/Debug.h"
#include "pingpong/core/Local.h"
#include "pingpong/core/Util.h"
#include "pingpong/core/Server.h"
#include "pingpong/core/User.h"

namespace PingPong {
	class Event {
		protected:
			bool isEmpty = false;
			Event(bool empty, const std::string &content_): isEmpty(empty), content(content_) {}

		public:
			long stamp = Util::timestamp();
			std::string content;

			virtual ~Event() = default;
			Event(): isEmpty(true) {}

			operator bool() const { return !isEmpty; }
	};

	using Listener_f = std::function<void(Event *)>;

	class FakeEvent: public Event { public: FakeEvent(): Event() {} };

	class Events {
		private:
			static std::multimap<std::string, std::pair<std::string, Listener_f>> listeners;
			static size_t listenersAdded;

		public:
			template <typename T>
			static void listen(const std::string &name, std::function<void(T *)> fn) {
				listeners.insert({std::string(typeid(T).name()), {
					name,
					[=](Event *event) {
						fn(dynamic_cast<T *>(event));
					}
				}});
			}

			template <typename T>
			static std::string listen(std::function<void(T *)> fn) {
				std::string name = "_" + std::to_string(++listenersAdded);
				listen(name, fn);
				return name;
			}

			template <typename T>
			static bool unlisten(const std::string &name) {
				auto range = listeners.equal_range(typeid(T).name());
				for (auto iter = range.first; iter != range.second; ++iter)
					if (iter->second.first == name) {
						listeners.erase(iter);
						return true;
					}

				return false;
			}

			template <typename T>
			static void dispatch(T *ptr) {
				auto range = listeners.equal_range(typeid(T).name());
				for (auto iter = range.first; iter != range.second; ++iter)
					iter->second.second(ptr);
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

	/** For events local to one server. */
	struct ServerEvent: public Event {
		Server *server;

		ServerEvent(Server *server_, const std::string &content_ = ""): Event(false, content_), server(server_) {}
	};

	/** For events local to one channel on one server. */
	struct ChannelEvent: public ServerEvent {
		std::shared_ptr<Channel> channel;

		ChannelEvent(const std::shared_ptr<Channel> &, Server *, const std::string & = "");
		ChannelEvent(const std::shared_ptr<Channel> &channel_, const std::string &content_ = ""):
			ChannelEvent(channel_, channel_? channel_->server : nullptr, content_) {}
	};

	/** For events local to one user in one channel on one server, such as joins.
	 *  This can also be used for things like quits, which are specific to a user and server but not to a channel, by
	 *  leaving the channel pointer null. */
	struct UserEvent: public ChannelEvent {
		std::shared_ptr<User> who;

		UserEvent(const std::shared_ptr<User> &, const std::shared_ptr<Channel> &, const std::string & = "");
		UserEvent(const std::shared_ptr<User> &who_, const std::string &content_ = ""):
			UserEvent(who_, nullptr, content_) {}
	};

	/** For events local on one server to either a user or a channel, such as privmsgs. */
	struct LocalEvent: public ServerEvent, public Local {
		template <typename T>
		LocalEvent(Server *server_, const T &where_, const std::string &content_ = ""):
			ServerEvent(server_, content_), Local(where_) {}
	};

	/** For events local to two users in one channel on one server, such as kicks. */
	struct TargetedEvent: public UserEvent {
		std::shared_ptr<User> whom;

		TargetedEvent(const std::shared_ptr<User> &, const std::shared_ptr<User> &, const std::shared_ptr<Channel> &,
			const std::string & = "");

		TargetedEvent(const std::shared_ptr<User> &who_, const std::shared_ptr<User> &whom_, Server *server_,
		const std::string &content_ = ""):
			TargetedEvent(who_, whom_, std::shared_ptr<Channel>(), content_) { server = server_; }
	};
}

#endif
