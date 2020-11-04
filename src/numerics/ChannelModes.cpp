#include "pingpong/core/Util.h"
#include "pingpong/core/ModeSet.h"

#include "pingpong/messages/Numeric.h"

namespace PingPong {
	bool NumericMessage::handleChannelModes(Server *server) {
		std::string chan_name, mode_str;
		std::tie(std::ignore, chan_name, mode_str) = parseSS(line);

		try {
			ModeSet modeset {ModeSet::ModeType::Channel, Util::rtrim(mode_str)};
			std::shared_ptr<Channel> channel {server->getChannel(chan_name, true)};

			// TODO: dispatch an event instead of just silently updating?
			channel->modes = modeset.added;
		} catch (const std::invalid_argument &err) {
			DBG("Failed to parse ModeSet (" << Util::rtrim(mode_str) << "): " << err.what());
		}

		return true;
	}
}
