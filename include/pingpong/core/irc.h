#ifndef PINGPONG_CORE_IRC_H_
#define PINGPONG_CORE_IRC_H_

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "pingpong/core/Defs.h"
#include "pingpong/core/Options.h"

#include "lib/formicine/ansi.h"

namespace PingPong {
	class IRC {
		using ConnectWrapper = std::function<void(const std::function<void()> &)>;

		private:
			std::mutex consoleMutex, serversMutex;

		public:
			static constexpr int defaultPort = 6667;
			static std::string defaultNick, defaultUser, defaultRealname;

			std::string username, realname;

			/** A map of server IDs to server instances. */
			std::map<std::string, Server *> servers {};

			std::list<Server *> serverOrder {};

			Server *activeServer = nullptr;

			/** Represents the client's version. */
			std::string version = "pingpong " PINGPONG_VERSION_NUMBER;

			IRC(std::string user, std::string real): username(user), realname(real) {}
			IRC(): IRC(defaultUser, defaultRealname) {}
			~IRC();

			/** Finds and returns the server with a given ID or nullptr if none is found. */
			Server * getServer(const std::string &id) const;

			/** Returns whether a server with a given ID exists. */
			bool hasServer(const std::string &id) const;

			/** Returns whether this instance contains a given server pointer. */
			bool hasServer(Server *) const;

			/** If this server contains a given server pointer, this function returns its key. Otherwise, it returns an
			 *  empty string. */
			std::string getKey(Server *) const;

			std::pair<std::string, long> connect(const std::string &where, const std::string &nick, long port = 6667,
			                                     ConnectWrapper wrapper = {});

			void init();
			void initMessages();

			/** Registers a set of standard event listeners. */
			void addListeners();

			std::unique_lock<std::mutex> lockConsole() { return std::unique_lock(consoleMutex); }
			std::unique_lock<std::mutex> lockServers() { return std::unique_lock(serversMutex); }

			/** Creates a unique ID for a server, a hostname. */
			std::string createID(const std::string &hostname);

			IRC & operator+=(Server *);
			IRC & operator-=(Server *);
	};
}

#endif
