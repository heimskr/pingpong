#include "messages/all.h"

namespace pingpong {
	template <typename S, typename C>
	std::pair<std::string, std::string> sourced_message<S, C>::parse_source_and_content(const std::string &combined) {
		std::string source;
		size_t i, length = combined.size();
		for (i = 0; combined[i] != ' ' && i < length; ++i)
			source += combined[i];

		for (; combined[i] == ' '; ++i);

		if (i == length || combined[i] != ':')
			throw std::runtime_error("Couldn't parse sourced_message");

		return {source, combined.substr(i + 1)};
	}

	template <>
	sourced_message<std::string, std::string>::sourced_message(const pingpong::line &line_,
	const std::string &combined): message(line_) {
		std::tie(source, content) = parse_source_and_content(combined.empty()? line_.parameters : combined);
	}
}
