#include "pingpong/core/util.h"
#include "pingpong/core/modeset.h"

#include "pingpong/messages/numeric.h"

namespace pingpong {
	bool numeric_message::handle_channel_modes(server *serv) {
		std::string chan_name, mode_str;
		std::tie(std::ignore, chan_name, mode_str) = parse_ss(line);

		try {
			modeset mset {modeset::mode_type::channel, util::rtrim(mode_str)};
			std::shared_ptr<channel> chan {serv->get_channel(chan_name, true)};

			// TODO: dispatch an event instead of just silently updating?
			chan->modes = mset.added;
		} catch (const std::invalid_argument &err) {
			DBG("Failed to parse modeset: " << err.what());
		}

		return true;
	}
}
