#ifndef PINGPONG_CORE_FEATURES_H_
#define PINGPONG_CORE_FEATURES_H_

#include <unordered_map>
#include <unordered_set>
#include <string>

namespace pingpong {
	struct features {
		enum class type {
			away_notify,      // Unimplemented
			batch,            // Unimplemented
			cap_notify,       // Unimplemented
			chghost,          // Unimplemented
			echo_message,     // Unimplemented
			extended_join,    // Unimplemented
			invite_notify,    // Unimplemented
			labeled_response, // Unimplemented
			message_ids,      // Unimplemented
			multi_prefix,
			server_time,      // Unimplemented
		};

		static std::unordered_set<type> implemented;
		static std::unordered_map<type, const char *> names;
		static std::unordered_map<std::string, type> types;
	};
}

#endif
