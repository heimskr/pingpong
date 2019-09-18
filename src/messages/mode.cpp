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

	void mode_message::validate_modes(const std::string &modes, const pingpong::line &line) {
		if (modes.empty())
			throw bad_message(line);

		// The first character has to be a plus or a minus.
		const char front = modes.front();
		if (front != '-' && front != '+')
			throw bad_message(line);

		// Next, check the string to see whether it never has a plus/minus immediately following another plus/minus
		// and contains nothing other than pluses/minuses and letters (uppercase or lowercase).
		
		bool was_pm = false; // Whether the last character was a plus or a minus.
		char last_mode = '?';
		for (char c: modes) {
			if (c == '+' || c == '-') {
				if (was_pm) // Can't have multiple pluses/minuses in a row.
					throw bad_message(line);
				was_pm = true;
				last_mode = c;
				continue;
			} else if (c < 'A' || 'z' < c || ('Z' < c && c < 'a')) {
				// Letters only, please.
				throw bad_message(line);
			}

			was_pm = false;

			// Now we add the characters to the removed/added sets and make sure no character is contained in both sets.
			if (last_mode == '+') {
				removed.erase(c);
				added.insert(c);
			} else if (last_mode == '-') {
				added.erase(c);
				removed.insert(c);
			} else {
				// Shouldn't ever happen.
				throw bad_message(line);
			}
		}

		// The last character can't be a plus or a minus; it has to be a letter.
		if (was_pm)
			throw bad_message(line);
	}

	void mode_message::apply() {

	}

	mode_message::operator std::string() const {
		return "(mode)";
	}
}
