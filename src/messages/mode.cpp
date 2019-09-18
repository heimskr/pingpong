#include "messages/mode.h"

namespace pingpong {
	mode_message::mode_message(const pingpong::line &line_): message(line_) {
		if (line.source.is_server() && line.source.nick == line.serv->get_nick()) {
			// If the server notifies you of a user mode change, the mask is just your nick without a user or host.
			// The parameters will look like "pingpong :+iwx".
			type = mode_type::self;
			size_t middle = line.parameters.find(" :");
			if (middle == std::string::npos)
				throw bad_message(line);
			
			modes = line.parameters.substr(line.parameters.find(" :") + 2);
			validate_modes(modes, line);
		} else {
			// This is presumably a channel mode change. The parameters should look like "#chan -S".
			type = mode_type::channel;
		}
	}

	void mode_message::apply() {

	}

	mode_message::operator std::string() const {
		return "(mode)";
	}
}
