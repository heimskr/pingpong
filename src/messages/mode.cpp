#include "messages/mode.h"

namespace pingpong {
	mode_message::mode_message(const pingpong::line &line_): message(line_) {
		if (line.source.is_server() && line.source.nick == line.serv->get_nick()) {
			// If the server notifies you of a user mode change, the mask is just your nick without a user or host.
			// The parameters will look like "pingpong :+iwx".
			size_t middle = line.parameters.find(" :");
			if (middle == std::string::npos)
				throw bad_message(line);
			
			mset = {modeset::mode_type::self, line.parameters.substr(middle + 2)};
		} else {
			// This is presumably a channel mode change. The parameters should look like "#chan -S".
			size_t middle = line.parameters.find(' ');
			if (middle == std::string::npos)
				throw bad_message(line);

			chan = line.serv->get_channel(line.parameters.substr(0, middle), true);
			mset = {modeset::mode_type::channel, line.parameters.substr(middle + 1)};
		}

		try {
			mset.process();
		} catch (const std::invalid_argument &err) {
			DBG("Couldn't parse \"" << mset.modes << "\" as a modestring: " << err.what());
			throw bad_message(line);
		}
	}

	bool mode_message::operator()(server *) {
		if (mset.type == modeset::mode_type::channel) {
			if (!chan)
				return false;
			chan->apply_modes(mset.removed, mset.added);
		} else {
			line.serv->get_self()->apply_modes(mset.removed, mset.added);
		}

		return true;
	}

	mode_message::operator std::string() const {
		return "(mode)";
	}
}
