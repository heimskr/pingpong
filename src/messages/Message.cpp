#include <memory>
#include <stdexcept>
#include <string>

#include "pingpong/core/Server.h"

#include "pingpong/events/Event.h"
#include "pingpong/events/Message.h"

#include "pingpong/messages/Numeric.h"

namespace PingPong {
	std::map<std::string, Message_ctor> Message::ctors;

	std::shared_ptr<Message> Message::parse(const PingPong::Line &line) {
		if (NumericMessage::isNumeric(line.command.c_str()))
			return std::make_shared<NumericMessage>(line);

		auto ctor = Message::ctors.find(line.command);
		if (ctor == Message::ctors.end())
			throw std::invalid_argument("Unknown message");

		return ctor->second(line);
	}

	Message::~Message() = default;

	Message::operator std::string() const {
		return std::string(line);
	}
}
