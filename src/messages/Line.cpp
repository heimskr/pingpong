#include <sstream>

#include "pingpong/messages/Line.h"
#include "pingpong/core/Mask.h"

#include "lib/formicine/ansi.h"
#include "lib/formicine/futil.h"

namespace PingPong {
	Line::Line(Server *server_, std::string in): original(in), server(server_), source(Mask("", "", "")) {
		size_t index = 0;

		if (in[0] == '@') {
			index = in.find(' ');
			const std::vector<std::string> tag_pairs = formicine::util::split(in.substr(1, index - 1), ";");
			in.erase(0, index + 1);
			for (const std::string &tag_pair: tag_pairs) {
				const size_t equals = tag_pair.find('=');
				if (equals != std::string::npos)
					tags.emplace(tag_pair.substr(0, equals), tag_pair.substr(equals + 1));
			}
		}

		if (in[0] == ':') {
			index = in.find(' ');
			source = Mask(in.substr(1, index - 1));
			in.erase(0, index + 1);
		}

		index      = in.find(' ');
		command    = in.substr(0, index);
		parameters = in.substr(index + 1);

		if (tags.count("time") == 0)
			time = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		else
			time = Util::parse8601(tags.at("time"));
	}
	
	Line::operator std::string() const {
		return "Tags[" + stringifyTags() + "], Source[" + std::string(source) + "], Command[" + command
			+ "], Parameters[" + parameters + "]";
	}

	Line::operator bool() const {
		return server && !original.empty();
	}

	std::string Line::stringifyTags() const {
		return stringifyTags(tags);
	}

	std::string Line::stringifyTags(const std::map<std::string, std::string> &tags) {
		if (tags.empty())
			return "";

		std::stringstream out;
		out << '@';
		bool first = true;
		for (const auto &[name, value]: tags) {
			if (first)
				first = false;
			else
				out << ';';
			out << name << '=' << value;
		}
		return out.str();
	}

	std::ostream & operator<<(std::ostream &os, const Line &line_) {
		return os << std::string(line_);
	}
}
