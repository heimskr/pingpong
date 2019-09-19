#include "events/mode.h"
#include "messages/mode.h"

namespace pingpong {
	mode_message::mode_message(const pingpong::line &line_): message(line_), local("") {
		size_t middle;
		if (line.source.is_server() && line.source.nick == line.serv->get_nick()) {
			// If the server notifies you of a user mode change, the mask is just your nick without a user or host.
			// The parameters will look like "pingpong :+iwx".
			middle = line.parameters.find(" :");
			if (middle == std::string::npos)
				throw bad_message(line);
			
			mset = {modeset::mode_type::self, line.parameters.substr(middle + 2)};
		} else {
			// This is presumably a channel mode change. The parameters should look like "#chan -S".
			middle = line.parameters.find(' ');
			if (middle == std::string::npos)
				throw bad_message(line);

			mset = {modeset::mode_type::channel, line.parameters.substr(middle + 1)};
		}

		where = line.parameters.substr(0, middle);
		who = line.serv->get_user(line.source, true);

		try {
			mset.process();
		} catch (const std::invalid_argument &err) {
			DBG("Couldn't parse \"" << mset.modes << "\" as a modestring: " << err.what());
			throw bad_message(line);
		}
	}

	bool mode_message::operator()(server *serv) {
		if (mset.type == modeset::mode_type::channel) {
			(chan? chan : serv->get_channel(where, true))->apply_modes(mset.removed, mset.added);
		} else {
			line.serv->get_self()->apply_modes(mset.removed, mset.added);
		}

		events::dispatch<mode_event>(serv, where, who, mset);
		return true;
	}

	mode_message::operator std::string() const {
		return "";
	}
}
