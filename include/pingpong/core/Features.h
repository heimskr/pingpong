#ifndef PINGPONG_CORE_FEATURES_H_
#define PINGPONG_CORE_FEATURES_H_

#include <unordered_map>
#include <unordered_set>
#include <string>

namespace PingPong {
	struct Features {
		enum class Type {
			AwayNotify,       // Unimplemented
			Batch,            // Unimplemented
			CapNotify,        // Unimplemented
			ChgHost,          // Unimplemented
			EchoMessage,      // Unimplemented
			ExtendedJoin,
			InviteNotify,     // Unimplemented
			LabeledResponse,
			MessageIDs,       // Unimplemented
			MultiPrefix,
			ServerTime,
			UserhostInNames,
		};

		static std::unordered_set<Type> implemented;
		static std::unordered_map<Type, const char *> names;
		static std::unordered_map<std::string, Type> types;
	};
}

#endif
