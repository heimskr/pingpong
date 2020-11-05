#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include <signal.h>
#include <unistd.h>

#include "pingpong/core/Debug.h"
#include "pingpong/core/Server.h"

#include "pingpong/commands/Cap.h"
#include "pingpong/commands/Nick.h"
#include "pingpong/commands/Pong.h"
#include "pingpong/commands/Quit.h"
#include "pingpong/commands/User.h"

#include "pingpong/events/BadLine.h"
#include "pingpong/events/Message.h"
#include "pingpong/events/NamesUpdated.h"
#include "pingpong/events/NickUpdated.h"
#include "pingpong/events/Raw.h"
#include "pingpong/events/ServerStatus.h"
#include "pingpong/events/UserAppeared.h"

#include "lib/formicine/futil.h"

namespace PingPong {
	Server::Server(IRC *parent_, const std::string &id_, const std::string &hostname_, int port_):
	parent(parent_), id(id_), hostname(hostname_), port(port_) {
		getlineMutex.lock();
	}


// Private instance methods


	void Server::workRead() {
		signal(SIGPIPE, SIG_IGN);
		negotiateCapabilities();

		std::string line;
		while (std::getline(*stream, line)) {
			// Remove the carriage return. It's part of the spec, but std::getline removes only the newline.
			if (line.back() == '\r')
				line.pop_back();

			try {
				handleLine(PingPong::Line(this, line));
			} catch (const std::invalid_argument &) {
				// Already dealt with by dispatching a BadLineEvent.
			} catch (const BadMessage &) {}
		}

		getlineMutex.unlock();
	}

	void Server::workReap() {
		// Bad things happen if the reader tries to close the connection on its own, so it's necessary to use another
		// thread to start the process of killing a server. The line_done condition variable causes the reaper to wait
		// until the reader leaves its try block and the temporary PingPong::Line's destructor is called.
		// If you were to directly call Server::remove from error_message::operator(), the server would be deleted
		// before the PingPong::Line's destructor could be called.
		std::unique_lock<std::mutex> death_lock(deathMutex);
		death.wait(death_lock);
		buffer->close();
		getlineMutex.lock();
		getlineMutex.unlock();
		*parent -= this;
	}

	void Server::handleLine(const PingPong::Line &line) {
		std::shared_ptr<Message> msg;
		try {
			msg = PingPong::Message::parse(line);
		} catch (const std::invalid_argument &err) {
			DBG("Bad line: " << err.what());
			Events::dispatch<BadLineEvent>(this, line.original);
			throw;
		} catch (const BadMessage &err) {
			DBG("Bad line: " << err.what());
			Events::dispatch<BadLineEvent>(this, line.original);
			throw;
		}

		Events::dispatch<RawInEvent>(this, line.original);
		if (!(*msg)(this))
			Events::dispatch<MessageEvent>(this, msg);
		lastMessage = msg;
	}

	void Server::negotiateCapabilities() {
		if (Features::implemented.empty()) {
			status = Stage::SetUser;
			UserCommand(this, parent->username, parent->realname).send();
		} else {
			CapCommand(this, {}, CapCommand::Action::Ls).send();
		}
	}


// Public instance methods


	void Server::quit(const std::string &message) {
		if (message.empty())
			PingPong::QuitCommand(this).send();
		else
			PingPong::QuitCommand(this, message).send();
	}

	bool Server::start() {
		auto lock(lockStatus());

		if (status != Stage::Unconnected)
			throw std::runtime_error("Can't connect: server not unconnected");

		sock   = std::make_shared<Net::Sock>(hostname, port);
		sock->connect();
		buffer = std::make_shared<Net::SocketBuffer>(sock.get());
		stream = std::make_shared<std::iostream>(buffer.get());

		const Stage old_status = status;
		status = Stage::CapNeg;
		if (status != old_status)
			Events::dispatch<ServerStatusEvent>(this);

		worker = std::thread(&Server::workRead, this);
		worker.detach();
		
		reaper = std::thread(&Server::workReap, this);
		reaper.detach();

		return true;
	}

	void Server::quote(const std::string &str) {
		if (!stream) {
			YIKES("Server::quote" >> ansi::style::bold << ": Stream not ready");
			throw std::runtime_error("Stream not ready");
		}

		auto lock(parent->lockConsole());
		Events::dispatch<RawOutEvent>(this, str);

		*stream << str << "\r\n";
		stream->flush();
	}

	void Server::setNick(const std::string &new_nick, bool immediate) {
		if (immediate) {
			nick = new_nick;
			getSelf()->rename(new_nick);
		} else {
			NickCommand(this, new_nick).send();
		}
	}

	Server::Stage Server::getStatus() {
		auto lock(lockStatus());
		return status;
	}

	void Server::setStatus(Stage new_status) {
		auto lock(lockStatus());
		if (new_status != status) {
			const Stage old_status = status;
			status = new_status;
			Events::dispatch<ServerStatusEvent>(this, old_status);
		}
	}

	std::string Server::statusString() const {
		switch (status) {
			case Stage::Unconnected: return "unconnected";
			case Stage::CapNeg: return "capneg";
			case Stage::SetUser: return "setuser";
			case Stage::SetNick: return "setnick";
			case Stage::Ready: return "ready";
			case Stage::Dead: return "dead";
		}

		return "unknown";
	}

	bool Server::addChannel(const std::string &chan) {
		if (!hasChannel(chan)) {
			channels.push_back(std::make_shared<Channel>(chan, this));
			return true;
		}

		DBG("Channel already exists: " << chan << "\r\n");
		return false;
	}

	bool Server::removeChannel(const std::shared_ptr<Channel> &chan) {
		if (!chan)
			return false;

		auto iter = std::find(channels.begin(), channels.end(), chan);
		if (iter == channels.end()) {
			DBG(ansi::color::red << "Channel pointer is inexplicably missing from server " << ansi::bold(id) << ": "
				<< chan->name);
			return false;
		}

		DBG("Removing channel " << chan->name);
		channels.erase(iter);

		return true;
	}

	bool Server::removeChannel(const std::string &chan) {
		std::shared_ptr<Channel> cptr = getChannel(chan, false);
		if (cptr)
			return removeChannel(cptr);
		DBG("Channel not in list: " << chan << "\n");
		return false;
	}

	bool Server::removeUser(const std::shared_ptr<User> &whom) {
		if (!whom)
			return false;

		auto iter = std::find(users.begin(), users.end(), whom);
		if (iter == users.end()) {
			DBG(ansi::color::red << "User pointer is inexplicably missing from server " << ansi::bold(id) << ": "
				<< whom);
			return false;
		}

		DBG("Removing user " << whom->name);
		users.erase(iter);

		return true;
	}

	bool Server::removeUser(const std::string &whom) {
		std::shared_ptr<User> uptr = getUser(whom, false, false);
		if (uptr)
			return removeUser(uptr);
		if (whom != "?")
			DBG("User not in list: " << whom << "\n");
		return false;
	}

	bool Server::hasChannel(const std::string &chanstr) const {
		for (std::shared_ptr<Channel> chan: channels) {
			if (chan->name == chanstr)
				return true;
		}

		return false;
	}

	bool Server::hasUser(std::shared_ptr<User> user) const {
		return std::find(users.begin(), users.end(), user) != users.end();
	}

	bool Server::hasUser(const std::string &name) const {
		const std::string lower = formicine::util::lower(name);
		for (std::shared_ptr<User> user: users) {
			if (formicine::util::lower(user->name) == lower)
				return true;
		}

		return false;
	}

	std::shared_ptr<Channel> Server::getChannel(const std::string &chanstr, bool create) {
		if (!hasChannel(chanstr)) {
			if (!create)
				return nullptr;
			addChannel(chanstr);
		}

		for (std::shared_ptr<Channel> chan: channels) {
			if (chan->name == chanstr)
				return chan;
		}

		return nullptr;
	}

	std::shared_ptr<User> Server::getUser(const std::string &rawname, bool create, bool update_case) {
		std::string name;
		Mask info {};
		if (rawname.find('!') != std::string::npos) {
			info = rawname;
			name = info.nick;
		} else {
			name = rawname;
		}

		if (!hasUser(name)) {
			if (!create)
				return nullptr;
			std::shared_ptr<User> new_user = std::make_shared<User>(name, this);
			new_user->info = info;
			users.push_back(new_user);
			Events::dispatch<UserAppearedEvent>(new_user);
			return new_user;
		}

		const std::string lower = formicine::util::lower(name);
		for (std::shared_ptr<User> user: users) {
			if (formicine::util::lower(user->name) == lower) {
				if (info)
					user->info = info;

				if (update_case && user->name != name) {
					const std::string old_name = user->name;
					user->name = name;
					Events::dispatch<NickUpdatedEvent>(user, old_name, name);
				}

				return user;
			}
		}

		return nullptr;
	}

	std::shared_ptr<User> Server::getUser(const Mask &mask_, bool create, bool update_case) {
		if (mask_.isServer()) {
			// If the host and user are empty, then it's presumably a message from the server (it'll look like
			// {nick="irc.example.com", user="", host=""}) and we return nullptr because there's no actual user here.
			return nullptr;
		}

		// Otherwise, pass it off to another overload.
		return getUser(mask_.nick, create, update_case);
	}


	void Server::renameUser(const std::string &old_nick, const std::string &new_nick) {
		if (old_nick == nick) {
			nick = new_nick;
			removeUser("?");
		}

		if (std::shared_ptr<User> uptr = getUser(old_nick, false, false))
			uptr->rename(new_nick);

		for (const std::shared_ptr<Channel> &channel: channels) {
			channel->renameUser(old_nick, new_nick);
			Events::dispatch<NamesUpdatedEvent>(channel);
		}
	}

	void Server::sortChannels() {
		channels.sort([&](std::weak_ptr<Channel> left, std::weak_ptr<Channel> right) -> bool {
			return left.lock()->name < right.lock()->name;
		});
	}

	bool Server::isActive() const {
		return status != Stage::Dead && status != Stage::Unconnected;
	}

	std::shared_ptr<User> Server::getSelf() {
		// If you don't have a nick yet, return a fake "?" user. Once you do have a nick, the fake user is removed.
		return getUser(nick.empty()? "?" : nick, true, false);
	}

	void Server::kill() {
		auto lock(lockStatus());
		if (status == Stage::Dead)
			return;

		buffer->close();
		setDead();
	}

	void Server::reap() {
		death.notify_all();
	}

	void Server::setDead() {
		auto lock(lockStatus());
		if (status == Stage::Dead)
			return;
		const Stage old_status = status;
		status = Stage::Dead;
		Events::dispatch<ServerStatusEvent>(this, old_status);
	}

	Server::operator std::string() const {
		return port != IRC::defaultPort? hostname + ":" + std::to_string(port) : hostname;
	}

	std::unique_lock<std::recursive_mutex> Server::lockStatus() {
		return std::unique_lock(statusMutex);
	}

	void Server::addFeature(Features::Type feature) {
		enabledFeatures.insert(feature);
	}

	void Server::supportFeature(Features::Type feature) {
		supportedFeatures.insert(feature);
	}

	void Server::supportFeatures(const std::string &str) {
		std::vector<std::string> caps = formicine::util::split(str, " ");
		for (const std::string &cap: caps) {
			if (0 < Features::types.count(cap))
				supportFeature(Features::types[cap]);
		}
	}

	bool Server::featureEnabled(Features::Type feature) const {
		return enabledFeatures.count(feature) != 0;
	}

	void Server::capAnswered(size_t count) {
		capsRequested -= count;
		if (status == Stage::CapNeg && capsRequested == 0) {
			CapCommand(this, CapCommand::Action::End).send();
			UserCommand(this, parent->username, parent->realname).send();
		}
	}
}
